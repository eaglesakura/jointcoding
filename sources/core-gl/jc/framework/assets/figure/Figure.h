/*
 * Figure.h
 *
 *  Created on: 2013/05/22
 */

#ifndef JCFWFIGURE_H_
#define JCFWFIGURE_H_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * メッシュ全体のGPUリソース
 */
class MeshResource: public Object {

    /**
     * 頂点オブジェクト
     */
    jc_sp<FigureVertices> vertices;

    /**
     * インデックスオブジェクト
     */
    jc_sp<IndexBufferObject> indices;
public:
    MeshResource() {
    }

    virtual ~MeshResource() {
    }

    /**
     * バッファの確保を行う
     */
    virtual void initialize(MDevice device) {
        vertices.reset(new FigureVertices(device));
        indices.reset(new IndexBufferObject(device));
    }

    /**
     * 頂点を取得する
     */
    jc_sp<FigureVertices> getVertices() const {
        return vertices;
    }

    /**
     * インデックスを取得する
     */
    jc_sp<IndexBufferObject> getIndices() const {
        return indices;
    }
};

/**
 * managed
 */
typedef jc_sp<MeshResource> MMeshResource;

/**
 * 1つの3Dモデル=フィギュアを構築する
 */
class Figure: public Object {
    /**
     * 全ノードの一括キャッシュ
     */
    safe_array<FigureNode*> nodes;

    /**
     * GPUリソース
     */
    MMeshResource resource;

public:
    Figure() {
    }

    virtual ~Figure() {
        jclogf("delete Figure(0x%x)", this);
        safe_delete(nodes);
    }

protected:

    /**
     * 指定したインデックスのノードを生成する
     */
    virtual FigureNode* createNode(const s32 index) {
        return new FigureNode(index);
    }

    /**
     * リソース管理クラスを生成する
     */
    virtual MeshResource* createResource() {
        return new MeshResource();
    }

public:
    /* for loader */

    /**
     * ノードテーブルを初期化する
     */
    virtual void initializeNodes(MDevice device, const u32 nodeNum) {
        // 指定数のノードを確保する
        nodes.reserve(nodeNum);
        nodes.zeromemory();

        // 指定数のノードを生成する
        for (s32 i = 0; i < (s32) nodeNum; ++i) {
            nodes[i] = createNode(i);
        }

        resource.reset(createResource());
        resource->initialize(device);
    }

    /**
     * ノードを取得する
     */
    virtual FigureNode* getNode(const u32 index) {
        return nodes[index];
    }

    /**
     * ノードを取得する
     */
    virtual const FigureNode* getNode(const u32 index) const {
        return nodes[index];
    }

    /**
     * ノード数を取得する
     */
    virtual u32 getNodeNum() const {
        return nodes.length;
    }

    /**
     * リソース管理クラスを取得する
     */
    virtual MMeshResource getResource() const {
        return resource;
    }
};

}
}

#endif /* JCFWFIGURE_H_ */
