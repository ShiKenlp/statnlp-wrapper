// Created by Shi Ke on 20/10/2018.
// This module provides python bindings for the statNLP C++ verison framework.

%module(directors="1") statnlp

// Required header files for compiling wrapped code.
%{
//#include "common.h"
#include "src/hybridnetworks/instance.h"
//#include "base_instance.h"
//#include "data_manager.h"
#include "src/hybridnetworks/feature_manager.h"
#include "src/hybridnetworks/feature_array.h"
#include "src/hybridnetworks/global_network_param.h"
#include "src/hybridnetworks/local_network_param.h"
#include "src/hybridnetworks/network_id_manager.h"
#include "src/hybridnetworks/table_lookup_network.h"
//#include "src/hybridnetworks/hypergraph.h"
#include "src/hybridnetworks/network.h"
#include "src/hybridnetworks/network_model.h"
#include "src/hybridnetworks/network_compiler.h"
#include "src/hybridnetworks/discriminative_network_model.h"
%}

//Required SWIG libraries.
%include "stl.i"
%include "std_string.i"
%include "typemaps.i"

// Declare explicit types for needed instantiations of generic types.
class Instance;
class FeatureArray;
//class Network;

namespace std{
  %template(VecInt) vector<int>;
  %template(VecLong) vector<long>;
  %template(VecIns) vector<Instance*>;
  %template(VecVecIns) vector<vector<Instance*>*>;
  %template(VecStr) vector<string>;
  %template(VecVecStr) vector<vector<string>>;
};

// The subset of classed/methods/functions we want to wrap.

//////////////////////////////////
// declarations from common.h //
//////////////////////////////////


//////////////////////////////////
// declarations from instance.h //
//////////////////////////////////
class Instance{
public:
    Instance(int id, double weight);
    bool IS_Labeled();
    void SetLabeled();
    void SetUnlabeled();
    double GetWeight();
    int GetInstanceId();
    void SetInstanceId(int id);
};

///////////////////////////////////////
// declarations from base_instance.h //
///////////////////////////////////////

//template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
//class BaseInstance:public Instance{
//public:
//    BaseInstance();
//    BaseInstance(SELF_TYPE *INPUT, INPUT_TYPE *INPUT, OUTPUT_TYPE *INPUT, int id, double weight):Instance(id,weight);
//    OUTPUT_TYPE *GetOutPut();
//    INPUT_TYPYE *GetInput();
//    OUTPUT_TYPE *GetPrediction();
//    void SetPrediction(OUTPUT_TYPE *ptr_prediction);
//};
//%template(BaseInstanceI) BaseInstance<int,int,int>;


//////////////////////////////////////////////////
//// declarations from local_network_param.h /////
//////////////////////////////////////////////////
class LocalNetworkParam{};


///////////////////////////////////////////
//// declarations from feature_array.h ////
///////////////////////////////////////////

%typemap(in) (int *ptr_fs, int fs_size) {
  int i;
  if (!PyList_Check($input)) {
    PyErr_SetString(PyExc_ValueError, "Expecting a list");
    return NULL;
  }
  $2 = PyList_Size($input);
  $1 = (int *) malloc(($2)*sizeof(int));
  for (i = 0; i < $2; i++) {
    PyObject *s = PyList_GetItem($input,i);
    if (!PyInt_Check(s)) {
        free($1);
        PyErr_SetString(PyExc_ValueError, "List items must be integers");
        return NULL;
    }
    $1[i] = PyInt_AsLong(s);
  }
};

%typemap(freearg) (int *ptr_fs, int fs_size) {
   if ($1) free($1);
};

class FeatureArray{
public:
%rename FeatureArray FeatureArray0;
    FeatureArray();
%rename FeatureArray FeatureArray1;
    FeatureArray(double score);
%rename FeatureArray FeatureArray2;
    FeatureArray(int *ptr_fs, int fs_size);
%rename FeatureArray FeatureArray3;
    FeatureArray(int *ptr_fs, int fs_size, FeatureArray *ptr_next);
    void GetFeatureArray();
};

%pythoncode %{
def FeatureArray(*arg):
  if (len(arg) == 0):
    return FeatureArray0(*arg)
  elif (len(arg) == 1):
    if (isinstance(*arg,list)):
      return FeatureArray2(*arg)
    else:
      return FeatureArray1(*arg)
  else:
    return FeatureArray3(*arg)
%}

////////////////////////////////////////////////
//// declarations from global_network_param.h //
////////////////////////////////////////////////
//%apply int& INPUT {int &argc};
//%apply char**& INPUT {char **&};
//%apply std::vector<std::string> *INPUT {std::vector<std::string> *ptr_label};
class GlobalNetworkParam{
public:
    GlobalNetworkParam(int vocab_size);
    int ToFeature(std::string type, std::string output, std::string input);
    int tmp_count_;
};


//////////////////////////////////////////////
//// declarations from feature_manager.h /////
//////////////////////////////////////////////
%feature("director") FeatureManager;
class FeatureManager{
public:
    //FeatureManager();
    FeatureManager(GlobalNetworkParam* ptr_parm);
    //virtual FeatureArray* ExtractHelper(Network *ptr_network, int parent, int *ptr_children) = 0;
    virtual FeatureArray* ExtractHelper(Network *ptr_network, int parent, std::vector<int> children_vec) = 0;
    int temp_count_;
};


//////////////////////////////////////////////
//// declarations from network_compiler.h ////
//////////////////////////////////////////////
%feature("director") NetworkCompiler;
class NetworkCompiler{
public:
    NetworkCompiler();
    virtual Network *Compile(int networkId, Instance* ptr_inst, LocalNetworkParam *ptr_param) = 0;
    virtual Instance *Decompile(Network *ptr_network) = 0;
};


//////////////////////////////////////////////////
//// declarations from network_id_manager.h //////
//////////////////////////////////////////////////
class NetworkIDManager{
public:
    NetworkIDManager();
    static long ToHybridNodeID(std::vector<int> &vec);
    static std::vector<int> ToHybridNodeArray(long NodeId);
};

////////////////////////////////////////
//// declarations from hypergraph.h ////
////////////////////////////////////////
class HyperGraph {
public:
    virtual long GetNode(int k) = 0;
    virtual int CountNodes() = 0;
    virtual std::vector<int> GetNodeArray(int k) = 0;
    virtual int **GetChildren(int k) = 0;
    virtual bool IsRemovded(int k) = 0;
    virtual void Remove(int k) = 0;
    virtual bool IsRoot(int k) = 0;
    virtual bool IsLeaf(int k) = 0;
    virtual bool IsContain(long node) = 0;
    //get the number of hyperedges rootd by node_index;
    virtual int GetChildrens_Size(int node_index) = 0;
    //get the pointer that stores the number of nodes in each hyperedge rooted by node_index.
    virtual int *GetChildren_Size(int node_index) = 0;
};

////////////////////////////////////////
//// declarations from network.h //////
///////////////////////////////////////
class Network : public HyperGraph {
public:
    Network();
    Instance *GetInstance();
    //int *GetMaxPath(int nodeid);
    std::vector<int> GetMaxpath(int nodeid);
    std::vector<int> GetNodeArray(int nodeIndex);
    virtual int CountNodes();
    void CreateFeatureArray(int *ptr_fs, int fs_size, bool is_first_array);
};

//////////////////////////////////////////////////
//// declarations from table_lookup_network.h ////
//////////////////////////////////////////////////
class TableLookupNetwork: public Network{
public:
    TableLookupNetwork();
    TableLookupNetwork(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param);
    TableLookupNetwork(int networkId, Instance *ptr_inst, TableLookupNetwork *ptr_network, LocalNetworkParam *ptr_param, int num_nodes);
    bool AddNode(long nodeId);
    void AddEdge(long parent, std::vector<long> &children);
    void FinalizeNetwork();
    long *GetAllNodes();
    int*** GetAllChildren();
    int CountNodes() override;
    long GetNode(int k) override;
    int **GetChildren(int k) override;
    bool IsRemovded(int k) override;
    void Remove(int k) override;
    bool IsRoot(int k) override;
    bool IsLeaf(int k) override;
    bool IsContain(long node) override;
    int GetChildrens_Size(int node_index) override;
    int *GetChildren_Size(int node_index) override;
    int BinarySearch(int array_size, long value);
};

////////////////////////////////////////////
//// declarations from network_model.h ////
///////////////////////////////////////////
class NetworkModel{
public:
    NetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc);
    void Train(std::vector<Instance *> *ptr_all_instances, std::vector<Instance *> *ptr_all_instances_du, int max_num_interations);
    std::vector<Instance *>* Decode(std::vector<Instance *> *ptr_test_instences, bool is_cache_features);
    virtual std::vector<std::vector<Instance*>*>* SplitInstanceForTrain() = 0;
};

//////////////////////////////////////////////////////////
//// declarations from discriminative_network_model.h ////
//////////////////////////////////////////////////////////
class DiscriminativeNetworkModel: public NetworkModel{
public:
    DiscriminativeNetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc);
    std::vector<std::vector<Instance*>*> *SplitInstanceForTrain();
};

// end of the wrapper file.
