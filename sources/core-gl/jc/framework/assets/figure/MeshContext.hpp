/*
 * MeshFragment.hpp
 *
 *  Created on: 2013/05/21
 */

#ifndef MESHFCONTEXT_HPP_
#define MESHFCONTEXT_HPP_

#include    "jc/framework/assets/figure/FigureAsset.hpp"

namespace jc {
namespace fw {

typedef VertexBufferObject<void> FigureVertices;

/**
 * メッシュの断片を構築する
 *
 * MeshContextは1つ以上組み合わせて１つのマテリアル部位を構築し、それらを組み合わせて１つのメッシュを構築する。
 * スキニングのボーン数制限を回避するため、このクラスが描画の最小単位となる。
 */
class MeshContext: public Object {
protected:
    struct {
        /**
         * 頂点オブジェクト
         */
        jc_sp<FigureVertices> vertices;

        /**
         * インデックスオブジェクト
         */
        jc_sp<IndexBufferObject> indices;
    }buffers;
public:
    MeshContext() {
    }

    virtual ~MeshContext() {
        jclogf("delete MeshContext(0x%x)", this);
    }

    /**
     * バッファの初期化を行う
     */
    virtual void initialize(MDevice device) {
        buffers.vertices.reset(new FigureVertices(device));
        buffers.indices.reset(new IndexBufferObject(device));
    }

    /**
     * 頂点バッファオブジェクトを取得する
     */
    virtual FigureVertices* getVertexBuffer() const {
        return buffers.vertices.get();
    }

    /**
     * インデックスバッファオブジェクトを取得する
     */
    virtual IndexBufferObject* getIndicesBuffer() const {
        return buffers.indices.get();
    }
};

}
}

#endif /* MESHFRAGMENT_HPP_ */
