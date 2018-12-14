//
// Created by shi ke on 2018/9/21.
//


#include "src/hybridnetworks/table_lookup_network.h"
#include "tree_crf_instance.h"

#ifndef STATNLP_TREE_CRF_NETWORK_H
#define STATNLP_TREE_CRF_NETWORK_H

class TreeCRFNetwork: public TableLookupNetwork{
public:
    TreeCRFNetwork();
    ~TreeCRFNetwork();

};



#endif //STATNLP_TREE_CRF_NETWORK_H
