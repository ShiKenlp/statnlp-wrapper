//
// Created by shi ke on 2018/9/20.
//

#ifndef STATNLP_TREE_CRF_NEWORK_COMPILER_H
#define STATNLP_TREE_CRF_NEWORK_COMPILER_H

#include <list>
#include <map>
#include <string>
#include "src/hybridnetworks/network_compiler.h"
#include "tree_crf_instance.h"
#include "tree_crf_network.h"

class TreeCRFNetworkCompiler: public NetworkCompiler{
public:
    TreeCRFNetworkCompiler();
    ~TreeCRFNetworkCompiler();
    
};

#endif //STATNLP_TREE_CRF_NEWORK_COMPILER_H
