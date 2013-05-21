/*
 * FigureBuffer.hpp
 *
 *  Created on: 2013/05/21
 */

#ifndef FIGUREBUFFER_HPP_
#define FIGUREBUFFER_HPP_

#include    "jc/framework/assets/figure/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * スキニング用頂点オブジェクト
 */
typedef VertexBufferObject<void> FigureVertices;

/**
 * Figure構築に必要なバッファを管理する
 *
 */
class FigureBuffer: public Object {
    /**
     * 頂点オブジェクト
     * バッファはひとつのみで十分
     */
    jc_sp<FigureVertices> vbo;

    /**
     * インデックスオブジェクト
     */
    jc_sp<IndexBufferObject> ibo;
public:
    FigureBuffer( ) {
    }

    virtual ~FigureBuffer() {

    }

    /**
     * 特定デバイス内にバッファを生成する
     * バッファの精製後、データの転送等はロード側で適当に行う
     */
    virtual void initializeBuffers(MDevice device) {
        vbo.reset(new FigureVertices(device));
        ibo.reset(new IndexBufferObject(device));
    }

    /**
     * 頂点バッファを取得する
     */
    FigureVertices* getVertices() const {
        return vbo.get();
    }

    /**
     * インデックスバッファオブジェクトを生成する
     */
    IndexBufferObject* getIndices() const {
        return ibo.get();
    }
};

}
}

#endif /* FIGUREBUFFER_HPP_ */
