//
// Created by shi ke on 2018/9/20.
//

#ifndef STATNLP_TREE_CRF_FEATURE_MANAGER_H
#define STATNLP_TREE_CRF_FEATURE_MANAGER_H

#include "src/hybridnetworks/feature_manager.h"
#include "src/hybridnetworks/instance.h"

enum FeatureType{
    LEFT_RIGHT,
    FIRST_WORD,
    SPLIT_WORD,
    LAST_WORD,
    LAST_WORD_ENDING_1,
    LAST_WORD_ENDING_2,
    LAST_WORD_ENDING_3,
    FIRST_CAPITAL,
};


class TreeCRFFeatureManager:public FeatureManager{
public:
    //TreeCRFFeatureManager();
    TreeCRFFeatureManager(GlobalNetworkParam *ptr_param);

};
#endif //STATNLP_TREE_CRF_FEATURE_MANAGER_H
//public:
////    LinearCRFFeatureManager();
//LinearCRFFeatureManager(GlobalNetworkParam *ptr_param, std::vector<Instance*> *ptr_inst);
//~LinearCRFFeatureManager();
//FeatureArray* ExtractHelper(Network *ptr, int parent, int *ptr_children);
//static int word_hal_window_size_;
//static std::string feature_type_[5];
//private:
//std::vector<Instance*> *ptr_inst_vector_;
//FeatureT *ptr_feature_type_;
//};