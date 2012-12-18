/*
 * prv_FbxDeformer.cpp
 *
 *  Created on: 2012/12/18
 */
#include    "prv_FbxDeformer.h"

namespace jc {
namespace fbx {

static void createBoneIndicesArray(std::vector<s32> *result, KFbxCluster *cluster) {
    s32* origin = cluster->GetControlPointIndices();
    s32 num = cluster->GetControlPointIndicesCount();

    for (s32 i = 0; i < num; ++i) {
        const s32 index = (s32) origin[i];
        result->push_back(index);
    }
}

//! ボーン情報
static void createBoneWeightArray(std::vector<float> *result, KFbxCluster *cluster) {
    double* origin = cluster->GetControlPointWeights();
    s32 num = cluster->GetControlPointIndicesCount();

    for (s32 i = 0; i < num; ++i) {
        float weight = (float) origin[i];
        result->push_back(weight);
    }

}

ConvertedDeformer::ConvertedDeformer(KFbxCluster *cluster) {
    createBoneIndicesArray(&indices, cluster);
    createBoneWeightArray(&weights, cluster);

    if (strlen(cluster->GetName())) {
        name = cluster->GetName() + (strlen("Cluster_"));
    }

    /*
     KFbxMatrix  m0 =    cluster->GetLink( )->GetGlobalFromDefaultTake( );
     KFbxMatrix  m1 =    cluster->GetLink( )->GetGlobalFromCurrentTake( KTime( ) );
     KFbxVector4 translate   = cluster->GetLink( )->GetLocalTFromDefaultTake( );
     KFbxVector4 rotate      = cluster->GetLink( )->GetLocalRFromDefaultTake( );
     KFbxVector4 scale       = cluster->GetLink( )->GetLocalSFromDefaultTake( );
     */
}

ConvertedDeformer::~ConvertedDeformer() {
}

}

}

