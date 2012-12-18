/*
 * prv_FbxDeformer.h
 *
 *  Created on: 2012/12/18
 */

#ifndef PRV_FBXDEFORMER_H_
#define PRV_FBXDEFORMER_H_

#include    "jointcoding-fbx.h"
#include    <vector>

namespace jc {
namespace fbx {

/**
 * FBXの１メッシュ毎に複数のDeformerが作成される。
 *
 * Deformerは特定の頂点に対して影響を及ぼす、ボーンとしての役割がある。
 *
 * indices[num]とweights[num]はセットになっていて、indices[num]の頂点に対してweights[num]を与える。
 */
class ConvertedDeformer: public Object {
public:
    /**
     * 頂点番号テーブル
     */
    std::vector<s32> indices;
    std::vector<float> weights; //!<    重みテーブル
    String name; //!<    ボーン名
    ConvertedDeformer(KFbxCluster *cluster);
    virtual ~ConvertedDeformer();
};

}
}

#endif /* PRV_FBXDEFORMER_H_ */
