//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_NETWORKIDMANAGER_H
#define STATNLP_NETWORKIDMANAGER_H

#include "common.h"


class NetworkIDManager{
public:
    NetworkIDManager();
    ~NetworkIDManager();
    static long ToHybridNodeID(std::vector<int> &vec);
    static std::vector<int> ToHybridNodeArray(long NodeId);
    static int BinarySearch(long *ptr_array, int array_size, long value);
private:
    static std::vector<int> *ptr_capacity_vec_;
};
#endif //STATNLP_NETWORKIDMANAGER_H
