import statnlp as sn
from enum import Enum
import codecs


class LinearCRFInstance(sn.Instance):

    def __init__(self, input, output, id, weight):
        super().__init__(id, weight)
        self.input = input
        self.output = output
        self.prediction = None


class LinearCRFNetwork(sn.TableLookupNetwork):

    def __init__(self, networkId=None, inst=None, param=None, net=None, num_nodes=-1):
        if networkId == None:
            super().__init__()
        elif net == None:
            super().__init__(networkId, inst, param)
        else:
            super().__init__(networkId, inst, net, param, num_nodes)


class NodeType(Enum):
    leaf = 0
    node = 1
    root = 2


class LinearCRFFeatureManager(sn.FeatureManager):

    def __init__(self, gnp, insts):
        super().__init__(gnp)
        self.gnp = gnp
        self.insts = insts

    def set_instances(self, insts):
        self.insts = insts

    def ExtractHelper(self, network, parent, children_vec):
        #print("The ExtractHelper in python was called")
        inst_id = abs(network.GetInstance().GetInstanceId())
        inst = self.insts[inst_id - 1]
        words = inst.input
        parent_node = network.GetNode(parent)
        parent_arr = sn.NetworkIDManager.ToHybridNodeArray(parent_node)
        parent_type = parent_arr[4]
        label_id = parent_arr[1]
        pos = parent_arr[0] - 1
        feature0 = sn.FeatureArray([])
        feature0.thisown = 0

        if parent_type == NodeType.leaf.value:
            return feature0

        child_node = network.GetNode(children_vec[0])
        child_label_id = sn.NetworkIDManager.ToHybridNodeArray(child_node)[1]
        feature_unigram_index = self.gnp.ToFeature("Unigram", str(label_id), words[pos][0])
        feature_unigram = sn.FeatureArray([feature_unigram_index], feature0)
        feature_unigram.thisown = 0
        feature_transition_index = self.gnp.ToFeature("Transition", str(label_id), str(child_label_id))
        feature_transition_unigram = sn.FeatureArray([feature_transition_index], feature_unigram)
        feature_transition_unigram.thisown = 0

        return feature_transition_unigram


class LinearCRFNetworkCompiler(sn.NetworkCompiler):

    def __init__(self, all_labels, label2idx_map, insts, insts_dup):
        super().__init__()
        self.all_labels = all_labels
        self.label2idx_map = label2idx_map
        self.insts = insts
        self.insts_dup = insts_dup
        self.max_size = 300
        self.net = None
        self.generic_net = None
        self.compiler_unlabeled_generic()

    def set_instances(self, insts):
        self.insts = insts
        self.insts_dup = insts

    def Compile(self, networkId, inst, param):
        #print("The Compile in python was called")
        if inst.IS_Labeled():
            return self.compiler_labeled(networkId, inst, param)
        else:
            return self.compiler_unlabeled(networkId, inst, param)

    def to_node(self, pos, label_id):
        return sn.NetworkIDManager.ToHybridNodeID(sn.VecInt([pos+1, label_id, 0, 0, NodeType.root.value]))

    def to_leaf_node(self):
        return sn.NetworkIDManager.ToHybridNodeID(sn.VecInt([0, 0, 0, 0, NodeType.leaf.value]))

    def to_root_node(self, size):
        return sn.NetworkIDManager.ToHybridNodeID(sn.VecInt([size, len(self.all_labels), 0, 0, NodeType.node.value]))

    def compiler_labeled(self, networkId, inst, param):
        self.net = LinearCRFNetwork(networkId, inst, param)
        inst_id = abs(inst.GetInstanceId())
        instance = self.insts[inst_id - 1]
        label_seq = instance.output
        size = len(label_seq)
        leaf = self.to_leaf_node()
        self.net.AddNode(leaf)

        pre_node = leaf
        for pos in range(size):
            curr_node = self.to_node(pos, self.label2idx_map[label_seq[pos]])
            self.net.AddNode(curr_node)
            self.net.AddEdge(curr_node, sn.VecLong([pre_node]))
            pre_node = curr_node
        root = self.to_root_node(size)
        self.net.AddNode(root)
        self.net.AddEdge(root, sn.VecLong([pre_node]))
        self.net.FinalizeNetwork()

        return self.net

    def compiler_unlabeled_generic(self):
        net = LinearCRFNetwork()
        leaf = self.to_leaf_node()
        net.AddNode(leaf)
        pre_nodes = [leaf]

        for pos in range(self.max_size):
            curr_nodes = []

            for k in range(len(self.all_labels)):
                curr_node = self.to_node(pos, k)
                net.AddNode(curr_node)
                curr_nodes.append(curr_node)

                for pre_node in pre_nodes:
                    net.AddEdge(curr_node, sn.VecLong([pre_node]))
            pre_nodes = curr_nodes
            root = self.to_root_node(pos+1)
            net.AddNode(root)
            for pre_node in pre_nodes:
                net.AddEdge(root, sn.VecLong([pre_node]))

        net.FinalizeNetwork()
        self.generic_net = net

    def compiler_unlabeled(self, networkId, inst, param):
        inst_id = abs(inst.GetInstanceId())
        inst = self.insts_dup[inst_id -1]
        size = len(inst.input)
        root = self.to_root_node(size)
        cn = self.generic_net.CountNodes()
        pos = self.generic_net.BinarySearch(cn, root)
        num_nodes = pos + 1

        self.net = LinearCRFNetwork(networkId, inst, param, self.generic_net, num_nodes)
        return self.net

    def Decompile(self, network):
        inst_id = abs(network.GetInstance().GetInstanceId())
        num_nodes = network.CountNodes()
        size = len(self.insts[inst_id-1].input)
        root = self.to_root_node(size)
        node_k = self.generic_net.BinarySearch(num_nodes, root)
        prediction = []
        for i in range(size-1, -1, -1):
            child_k = network.GetMaxpath(node_k)[0]
            node = network.GetNode(child_k)
            child_arr = sn.NetworkIDManager.ToHybridNodeArray(node)
            label_id = child_arr[1]
            label = self.all_labels[label_id]
            prediction.append(label)
            node_k = child_k
        prediction.reverse()
        self.insts[inst_id - 1].prediction = prediction
        return self.insts[inst_id - 1]


class LinearCRFNetworkModel(sn.DiscriminativeNetworkModel):

    def __init__(self, fm, compiler):
        super().__init__(fm, compiler)


def read_data(file_path, is_labeled, is_duplicate):
    file = codecs.open(file_path, 'r', 'utf-8')
    instances = []
    index = 0
    sentence = []
    labels = []

    for line in file:
        tokens = line.strip().split()
        if len(tokens) > 2:
            word, tag, label = tokens
            sentence.append([word, tag])
            labels.append(label)
            if is_labeled:
                if label not in label2idx_map:
                    label2idx_map[label] = len(all_labels)
                    all_labels.append(label)
        else:
            index += 1
            if is_labeled:
                if not is_duplicate:
                    instance = LinearCRFInstance(sentence, labels, index, 1.0)
                    instance.SetLabeled()
                else:
                    instance = LinearCRFInstance(sentence, labels, -index, 1.0)
                    instance.SetUnlabeled()
            else:
                instance = LinearCRFInstance(sentence, labels, index, 1.0)
                instance.SetUnlabeled()
            instances.append(instance)
            sentence = []
            labels = []
    file.close()
    return instances


if __name__ == "__main__":
    train_file_path = "../data/conll2000/sample_part_train.txt"
    test_file_path = "../data/conll2000/sample_part_test.txt"
    all_labels = []
    label2idx_map = {}
    train_instances = read_data(train_file_path, 1, 0)
    train_instances_duplicate = read_data(train_file_path, 0, 1)
    test_instances = read_data(test_file_path, 0, 0)
    print("Read Data Finished.")
    max_iteration = 1000
    gnp = sn.GlobalNetworkParam(len(train_instances))
    gnp.thisown = 0
    fm = LinearCRFFeatureManager(gnp, train_instances).__disown__()

    compiler = LinearCRFNetworkCompiler(all_labels, label2idx_map, train_instances, train_instances_duplicate).__disown__()
    model = LinearCRFNetworkModel(fm, compiler)
    model.Train(sn.VecIns(train_instances), sn.VecIns(train_instances_duplicate), max_iteration)
    print("Train Finished.")

    fm.set_instances(test_instances)
    compiler.set_instances(test_instances)

    model.Decode(sn.VecIns(test_instances), False)
    print("Decode Finished.")
    num_correct = 0
    total_num = 0
    for inst in test_instances:
        for i in range(len(inst.output)):
            total_num += 1
            if inst.output[i] == inst.prediction[i]:
                num_correct += 1
    accuracy = num_correct / total_num
    print("accuracy: ", accuracy)

