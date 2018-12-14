//
// Created by shi ke on 2018/9/20.
//

#ifndef STATNLP_TREE_CRF_INSTANCE_H
#define STATNLP_TREE_CRF_INSTANCE_H

#include <vector>
#include "src/hybridnetworks/base_instance.h"
#include "src/hybridnetworks/common.h"
#include "binary_tree.h"

//template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>

class TreeCRFInstance: public BaseInstance<TreeCRFInstance, ComType::Input_Str_Vector, Node>{
public:
    TreeCRFInstance();
    TreeCRFInstance(int instanceId, double weight, ComType::Input_Str_Vector *ptr_sentence, Node *Ptr_tree);
    ~TreeCRFInstance();

    int GetSize();
};

#endif //STATNLP_TREE_CRF_INSTANCE_H