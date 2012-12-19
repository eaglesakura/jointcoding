/*
 * MeshFragment.h
 *
 *  Created on: 2012/12/18
 */

#ifndef JCFBXMESHFRAGMENT_H_
#define JCFBXMESHFRAGMENT_H_

#include    "jointcoding-fbx.h"
#include    "jc/graphics/figure/mesh/Figure.h"
#include    <vector>
#include    <map>

#include    "jcfbx/attribute/IndicesContainer.h"
#include    "jcfbx/attribute/VertexContainer.h"

namespace jc {
namespace fbx {

/**
 * メッシュを構築する一部分を管理する。
 * この配下にあるメッシュは全て同一マテリアルで描画する。
 * 使用するボーン数が規程を超えた場合、複数に分割する
 */
class MeshFragmentConverter {

    /**
     * １つのフラグメントが利用できる最大ボーン数
     */
    s32 maxBones;

    struct FragmentContext {

        /**
         * 頂点インデックス
         */
        std::vector<u16> indices;

        /**
         * 使うボーン番号
         */
        std::map<u8, u8> useBoneIndices;

        /**
         * 描画用の頂点テーブル
         */
        std::vector<FigureVertex> vertices;
    };
    typedef jc_sp<FragmentContext> MFragmentContext;

    /**
     * 分割したコンテキスト
     */
    std::vector< MFragmentContext > contexts;
private:

    /**
     * 現在の操作対象コンテキストを取得する
     */
    inline FragmentContext* getCurrentContext() const {
        return contexts[contexts.size() - 1].get();
    }
protected:

    /**
     * 既にボーンを使用済みか
     */
    virtual jcboolean isUsingBone(u8 index);

    /**
     * 利用できるボーン数をオーバーする場合true
     */
    virtual jcboolean isOverBone(const u8 *bones, const s32 bones_length);

    /**
     * 現在のデータから分離を行い、新たなフラグメントとして子を作成する。
     */
    virtual void separation();

    /**
     *
     */
    MeshFragmentConverter();

    /**
     * インデックスバッファを追加する
     */
    virtual void addIndices(const std::vector<FigureVertex> &vertices, const u16 p0, const u16 p1, const u16 p2);

    /**
     * ポリゴン数を数える。
     * 必ず三角ポリゴンで格納されるため、３で割るだけでいい
     */
    virtual s32 getPolygonCount(const int contextNumber) const {
        return contexts[contextNumber]->indices.size() / 3;
    }

    /**
     * 共通のFigureFragmentに変換する
     */
    virtual MFigureMeshFragment createFigureFragment( ) const;

    /**
     * フラグメント全体のポリゴン数を取得する
     */
    virtual s32 getPolygonCount() const {
        s32 result = 0;
        for( int i = 0; i < (int)contexts.size(); ++i ) {
            result += contexts[i]->indices.size();
        }

        return (result / 3);
    }

    /**
     * 利用するボーン数を取得する
     */
    virtual s32 getBoneCount(const int contextNumber) const {
        return contexts[contextNumber]->useBoneIndices.size();
    }

    /**
     * 頂点数を取得する
     */
    virtual s32 getVerticesCount(const int contextNumber) const {
        return contexts[contextNumber]->vertices.size();
    }

    /**
     * コンテキスト数を取得する
     */
    virtual s32 getContextCount() const {
        return contexts.size();
    }

public:
    /**
     *
     */
    virtual ~MeshFragmentConverter();

    /**
     * FBX Mesh -> JointCoding Meshに変換を行う
     */
    static void convertMesh( std::vector< MFigureMeshFragment > *result, const VertexContainer &vertices, const IndicesContainer &indices );
};

/**
 * managed
 */
typedef jc_sp<MeshFragmentConverter> MMeshFragmentConverter;

}
}

#endif /* MESHGROUP_H_ */
