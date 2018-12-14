//
// Created by  ngs on 01/08/2018.
//

#include "network.h"
#include "common.h"
#include <ctime>

//double **Network::ptr_inside_shared_array_ = new double*[ComParam::Num_Of_Threads];
//double **Network::ptr_outside_shared_array_ = new double*[ComParam::Num_Of_Threads];

Network::Network() {
    //std::cout << "no param"<<std::endl;
}

Network::Network(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param_l) {
//    std::cout<< "table lookup - network"<<std::endl;
    this->network_id_ = networkId;
    this->thread_id_ = ptr_param_l->GetThreadId();
    this->ptr_inst_ = ptr_inst;
    this->weight_ = this->ptr_inst_->GetWeight();
    this->ptr_param_l_ = ptr_param_l;
    this->ptr_param_g_ = ptr_param_l_->GetFeatureManager()->GetGlobalParam();
    tmp_count_ = 0;
}

Network::~Network() {
//    delete []ptr_max_;
//    //the content of ptr_max_children_no_ are pointers, which are allocated in the class TableLookupNetwork
//    delete []ptr_max_children_no_;
}

void Network::Touch() {
#ifdef GLOBAL
    std::lock_guard<std::mutex> mtx_locker(mtx);
#endif
    for (int k = 0; k < this->CountNodes(); ++k) {
        this->Touch(k);
    }
}

void Network::Touch(int node_index) {
//    std::cout<<"before IsRemovded."<<std::endl;
    if(this->IsRemovded(node_index)){
        return;
    }
//    std::cout<<"after IsRemovded."<<std::endl;
    //ptr_childrens_vec points to all hypedges which are rooted by node_index;
    int **ptr_childrens_vec = this->GetChildren(node_index);
    if(nullptr == ptr_childrens_vec){
        tmp_count_++;
        return;
    }
    //childrens_num is the num of hyperedges which are rooted by node_index;
    int childrens_num  = this->GetChildrens_Size(node_index);
    for(int children_index = 0; children_index < childrens_num; ++children_index){
        //int * ptr_children_num = this->GetChildren_Size(children_index);
        int *ptr_children_k = ptr_childrens_vec[children_index];
        //std::cout<<"before Extract in Touch:"<<std::endl;
        //ptr_children_k is the pointer of a hyperedge.
        this->ptr_param_l_->Extract(this,node_index,ptr_children_k,children_index);
        //std::cout<<"After Extract Touch"<<std::endl;
        tmp_count_++;
    }
}

void Network::Train() {
    if(this->weight_ == 0){
        return;
    }
    //long starttime = clock();
    this->Inside();
    //long Inside_time = clock();
    //Inside_time_ += Inside_time - starttime;
    this->Outside();
    //long Outside_time = clock();
    //Outside_time_ += Outside_time - Inside_time;
    this->UpdateGradient();
    long Gradeient_time = clock();
    //Gradient_time_ += Gradeient_time - Outside_time;
    //std::cout<<"Inside_time "<<Inside_time_*1.0/CLOCKS_PER_SEC<<std::endl;
    //std::cout<<"Outside_time "<<Outside_time_*1.0/CLOCKS_PER_SEC<<std::endl;
    //std::cout<<"Gradient_time "<<Gradient_time_*1.0/CLOCKS_PER_SEC<<std::endl;
    //std::cout<<"time_constant:"<< CLOCKS_PER_SEC<<std::endl;
    this->ptr_param_l_->AddObj(this->GetInside(this->CountNodes()-1) * weight_);
}

/**
 *
 * SharedArray is a temporary array to temporarily store the inside and outside value of a network.
 * It is dynamically allocated and can be extended according to maximum number of the nodes in a thread.
 *
 * @return
 */
double* Network::GetInsideSharedArray(){
    /*
    double *pptr_inside_array_ = ptr_param_g_->GetInsideSharedArray(this->thread_id_);
    //int *ptr_array_size = ptr_param_g_->GetInsideSharedArraySize();
    int num_node = this->CountNodes();
    if(nullptr == pptr_inside_array_[this->thread_id_]){
        //ptr_inside_shared_array_[this->thread_id_] = new double[this->CountNodes()];
        pptr_inside_array_[this->thread_id_] = new double[num_node];
        for(int i=0; i<num_node; ++i){
            pptr_inside_array_[this->thread_id_][i] = 0.0;
        }
        ptr_array_size[this->thread_id_] = num_node;
    }
    return pptr_inside_array_[this->thread_id_];*/
}

double* Network::GetOutsideSharedArray() {
    double **pptr_outside_array = ptr_param_g_->GetOutsideSharedArray();
    int *ptr_array_size = ptr_param_g_->GetOutsideSharedArraySize();
    int num_node = this->CountNodes();
    if(!pptr_outside_array[this->thread_id_] || num_node > ptr_array_size[this->thread_id_] ){
        pptr_outside_array[this->thread_id_] = new double[num_node];
        //init the value of shared array
        for(int i = 0; i < num_node; ++i){
            pptr_outside_array[this->thread_id_][i] = 0.0;
        }
        ptr_array_size[this->thread_id_] = num_node;
    }
    return pptr_outside_array[this->thread_id_];
}

int Network::GetNetworkID() {
    return network_id_;
}

Instance* Network::GetInstance() {
    return ptr_inst_;
}

std::vector<int> Network::GetNodeArray(int nodeIndex) {
    long nodeId = this->GetNode(nodeIndex);
    return NetworkIDManager::ToHybridNodeArray(nodeId);
}

double Network::GetInside(int nodeId) {
    if(nullptr == ptr_inside_){
        std::cerr<< "ERROR: the empty pointer of ptr_inside"<<std::endl;
    }
    return ptr_inside_[nodeId];
}

void Network::Max() {
    int num_count = this->CountNodes();
    //std::cout<<"The countnode in max:"<<num_count<<std::endl;
    ptr_max_ = new double[num_count];
    ptr_max_children_no_ = new int*[num_count];
    //init the value and the pointer.
    for(int nodeid = 0; nodeid < num_count; ++nodeid){
        ptr_max_[nodeid] = ComParam::DOUBLE_NEGATIVE_INFINITY;
        ptr_max_children_no_[nodeid] = nullptr;
    }
    //
    for(int nodeid = 0; nodeid < num_count; ++nodeid){
        this->Max(nodeid);
    }
}

void Network::Max(int nodeId) {
    if(this->IsRemovded(nodeId)){
        this->ptr_max_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
        return;
    }
    //for the leaf node.
//    if(0 == nodeId){
//        return;
//    }
    if(IsSumNode(nodeId)){
        //TODO:
    } else{
        //get the hyperedge num of nodeId
        int childrens_size = this->GetChildrens_Size(nodeId);
        //this pointer store the size of children for each hyperedge.
        int* ptr_children_size = this->GetChildren_Size(nodeId);
        //Get all hyperedges rooted by nodeId.
        int **ptr_childrens = this->GetChildren(nodeId);
        //Get the max value for each hyperedge rooted by nodeID.
        for(int children_k = 0; children_k < childrens_size; ++children_k){
            int *ptr_children_k = ptr_childrens[children_k];
            int size = ptr_children_size[children_k];
            if(IsIngored(ptr_children_k,size)){
                continue;
            }
            FeatureArray *ptr_fa = this->ptr_param_l_->Extract(this,nodeId,ptr_children_k,children_k);
            double score = ptr_fa->GetScore(ptr_param_l_);
            //std::cout<<"score before + :"<< score<< "size:"<< size<<std::endl;

            for(int i=0; i < size; ++i){
                score += this->ptr_max_[ptr_children_k[i]];
            }
            //std::cout<<"children K:"<< children_k<< " nodeid:" << nodeId<< " score: "<< score<<std::endl;
            if(score > ptr_max_[nodeId]){
                ptr_max_[nodeId] = score;
                ptr_max_children_no_[nodeId] = ptr_children_k;
            }
        }
    }
}

bool Network::IsSumNode(int nodeid) {
    return false;
}

bool Network::IsIngored(int *ptr_children, int size) {
    //std::cout<<"Network::Isingored"<<std::endl;
    for(int i = 0; i < size; ++i){
        if(IsRemovded(ptr_children[i])){
            //std::cout<<"before return Network::isIngored true"<<std::endl;
            return true;
        }
    }
    //std::cout<<"before return Network::isIngored false"<<std::endl;
    return false;
}

int* Network::GetMaxPath(int nodeid) {
    //std::cout<<"Before Get Maxpath in int*: "<< std::endl;
    return ptr_max_children_no_[nodeid];
}

std::vector<int> Network::GetMaxpath( int nodeid) {
    std::vector<int> vec;
    int children_size = GetChildrens_Size(nodeid);
    if (nullptr == ptr_max_children_no_[nodeid]){
        //std::cout<<"it is a nullptr"<<std::endl;
    }
    for (int i=0; i<children_size; i++){
        vec.push_back(ptr_max_children_no_[nodeid][i]);
    }
    return vec;
}

/**
 * caution: the memory allocated here is not released.
 * @param ptr_fs
 * @param fs_size
 * @param is_firstarray
 */
void Network::CreateFeatureArray(int *ptr_fs, int fs_size, bool is_firstarray) {
    if(is_firstarray){
        ptr_cur_fa_ = new FeatureArray(ptr_fs,fs_size);
    } else{
        ptr_cur_fa_ = new FeatureArray(ptr_fs,fs_size,ptr_prev_fa_);
    }
    ptr_prev_fa_ = ptr_cur_fa_;
}