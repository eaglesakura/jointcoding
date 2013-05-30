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
 * FigureRendererに引き渡され、描画時の情報として利用される。
 *
 *
 * * 内部で階層構造を完全に管理し、外部の保持や入れ替える必要のないクラス（NodeやGroup等）についてはナマのポインタによる直接管理を行う。
 */
class MeshResource: public Object {
protected:
    /**
     * 頂点オブジェクト
     */
    jc_sp<FigureVertices> vertices;

    /**
     * インデックスオブジェクト
     */
    jc_sp<IndexBufferObject> indices;

    /**
     * マテリアル管理
     */
    safe_array<MeshMaterial*> materials;

    /**
     * マテリアルを新規に生成する
     */
    virtual MeshMaterial* createMaterial() {
        return new MeshMaterial();
    }

public:
    MeshResource() {
    }

    virtual ~MeshResource() {
        jclogf("delete MeshResource(0x%x)", this);
        safe_delete(materials);
    }

    /**
     * バッファの確保を行う
     */
    virtual void initialize(MDevice device, const u32 materialNum) {
        vertices.reset(new FigureVertices(device));
        indices.reset(new IndexBufferObject(device));

        materials.alloc(materialNum);
        for(s32 i = 0; i < materialNum; ++i) {
            materials[i] = createMaterial();
        }
    }

    /**
     * 名前の無いマテリアルを取得する
     */
    virtual MeshMaterial* getFreeMaterial() const {
        for(s32 i = 0; i < materials.length; ++i) {
            if(materials[i] && !materials[i]->hasName()) {
                return materials[i];
            }
        }
        return NULL;
    }

    /**
     * マテリアルを検索する
     * 名前一致で検索するため、動作は遅いが確実にヒットする
     */
    virtual MeshMaterial* getMaterial(const String &name) const {
        for(s32 i = 0; i < materials.length; ++i) {
            if(materials[i] && materials[i]->getName() == name) {
                return materials[i];
            }
        }
        return NULL;
    }

    /**
     * マテリアルを検索する。ヒットしなかった場合、新しい新規にマテリアルを設定して返す。
     * 主に初期化のために提供する。
     */
    virtual MeshMaterial* getMaterialOrFree(const String &name) {
        // マテリアルを探す
        MeshMaterial* result = getMaterial(name);
        if(result) {
            return result;
        }

        // 空きを探す
        result = getFreeMaterial();
        if(result) {
            result->setName(name);
        }

        return result;
    }

    /**
     * マテリアルを検索する
     * ハッシュ値で検索するため、動作は速いが場合によってはハッシュ衝突が発生する
     */
    virtual MeshMaterial* getMaterialFast(const String &name) const {
        const u32 uniqueId = name.hashCode();
        for(s32 i = 0; i < materials.length; ++i) {
            if(materials[i] && materials[i]->getUniqueId() == uniqueId) {
                return materials[i];
            }
        }
        return NULL;
    }

    /**
     * 頂点を取得する
     */
    virtual jc_sp<FigureVertices> getVertices() const {
        return vertices;
    }

    /**
     * インデックスを取得する
     */
    virtual jc_sp<IndexBufferObject> getIndices() const {
        return indices;
    }
};

/**
 * managed
 */
typedef jc_sp<MeshResource> MMeshResource;

/**
 * 1つの3Dモデル=フィギュアを構築する
 * Figureは頂点バッファ、インデックスバッファ、テクスチャ、メッシュの階層情報を記録した巨大なコンテナとしてみなせる。
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
    virtual void initialize(MDevice device, const u32 nodeNum, const u32 materialNum) {
        // 指定数のノードを確保する
        nodes.reserve(nodeNum);
        nodes.zeromemory();

        // 指定数のノードを生成する
        for (s32 i = 0; i < (s32) nodeNum; ++i) {
            nodes[i] = createNode(i);
        }

        resource.reset(createResource());
        resource->initialize(device, materialNum);
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
