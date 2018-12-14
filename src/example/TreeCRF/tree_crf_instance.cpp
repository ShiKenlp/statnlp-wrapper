//
// Created by shi ke on 2018/9/20.
//

#include "tree_crf_instance.h"

TreeCRFInstance::TreeCRFInstance(int instanceId, double weight, ComType::Input_Str_Vector *ptr_sentence, Node *ptr_tree)
        :BaseInstance<TreeCRFInstance, ComType::Input_Str_Vector, Node> (this,ptr_sentence,ptr_tree,instanceId, weight){

};

TreeCRFInstance::TreeCRFInstance(){

}

TreeCRFInstance::~TreeCRFInstance(){

}

int TreeCRFInstance::GetSize() {
    return ptr_input_->size();
}