//
// Created by  ngs on 02/08/2018.
//

#ifndef STATNLP_FEATURE_ARRAY_H
#define STATNLP_FEATURE_ARRAY_H

#include <vector>

class LocalNetworkParam;

/**
 *  A hyperedge is consisted of several FeatureArray Instances, which is a list connected by the @addindex @anchor @class FeatureArray ptr_next
 */
class FeatureArray{
public:
    FeatureArray();
    FeatureArray(int *ptr_fs, int fs_size);
    FeatureArray(int *ptr_fs, int fs_size, FeatureArray *ptr_next);
    FeatureArray(double score);
    ~FeatureArray();
    double GetScore(LocalNetworkParam *ptr_local_param);
    double ComputeScore(LocalNetworkParam *ptr_local_param, int *ptr_fs);
    int * GetCurrent();
    void Update(LocalNetworkParam *ptr_param, double count);
    static FeatureArray *PTR_EMPTY;
    FeatureArray *ToLocal(LocalNetworkParam *ptr_param);

    // the blow for python wrapper:
    int GetFeatureSize();
    void GetFeatureArray();
private:
    double score_;
    int *ptr_fs_;
    //for global mode with one only on thread, the fs_size_ will be very small. while for the local mode with
    //multiple threads, the fs_size_ should be big enough. i.e., 1/2 of the overall feature num.
    int fs_size_;
    FeatureArray *ptr_next_;
    bool islocal_;
};

#endif //STATNLP_FEATURE_ARRAY_H
