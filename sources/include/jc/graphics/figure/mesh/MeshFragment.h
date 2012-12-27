/*
 * MeshFragment.h
 *
 *  Created on: 2012/12/18
 */

#ifndef MESHFRAGMENT_H_
#define MESHFRAGMENT_H_

#include    "jointcoding.h"
#include    <vector>

namespace jc {

/**
 * メッシュの断片を構築する。
 * Mesh( 1 object )
 *   L MeshFragment( change material )
 *      L FragmentContext ( 1 draw call <= 32 bones )
 *      L FragmentContext ( 1 draw call <= 32 bones )
 *      L FragmentContext ( 1 draw call <= 32 bones )
 *   L MeshFragment( change material )
 *      L FragmentContext ( 1 draw call <= 32 bones )
 *   L MeshFragment( change material )
 *      L FragmentContext ( 1 draw call <= 32 bones )
 */
template<typename VertexType, typename MaterialType, typename VertexBufferType = jc_sa<VertexType>, typename IndexBufferType = jc_sa<u16> >
class _MeshFragment {
public:
    /**
     * １描画ごとの情報を格納する
     */
    struct DrawingContext {
        /**
         * 描画対象の頂点配列
         */
        VertexBufferType vertices;

        /**
         * バーテックスバッファの長さ
         */
        u32 vertices_length;

        /**
         * 描画対象のインデックスバッファ
         */
        IndexBufferType indices;

        /**
         * インデックスバッファの長さ
         */
        u32 indices_length;

        /**
         * ボーン行列のピックアップテーブル
         * i番目の行列マトリクスは bone_matrix[ bonePickTable[i] ] を参照する
         */
        jc_sa<u8> bone_pick_table;

        /**
         * ボーンピックアップテーブルの長さ
         */
        u32 bone_pick_table_length;

        DrawingContext() {
            vertices_length = 0;
            indices_length = 0;
            bone_pick_table_length = 0;
        }
    };
private:
    /**
     * 描画用コンテキストリスト
     */
    std::vector< jc_sp<DrawingContext> > contexts;

    /**
     * 管理するマテリアル
     */
    jc_sp<MaterialType> material;

public:
    _MeshFragment() {
        material.reset( new MaterialType() );
    }

    virtual ~_MeshFragment() {
    }

    /**
     * 描画コンテキスト数を取得すう
     */
    virtual int getDrawingContextCount() const {
        return contexts.size();
    }

    /**
     * 描画コンテキストを追加する
     */
    virtual void addDrawingContext( jc_sp<DrawingContext> context ) {
        contexts.push_back(context);
    }

    /**
     * 描画コンテキスト数を再設定する
     */
    virtual void resizeDrawingContextCount(const u32 num) {
        contexts.resize(num);

        for( u32 i = 0; i < num; ++i ) {
            jc_sp<DrawingContext> ctx = contexts[i];
            if(!ctx) {
                contexts[i].reset(new DrawingContext());
            }
        }
    }

    /**
     * 描画コンテキストを取得する
     */
    virtual jc_sp<DrawingContext> getDrawingContext( const int index ) const {
        return contexts[index];
    }

    /**
     * 三角ポリゴン数を数える
     */
    virtual int getPolygonCount() const {
        int result = 0;
        const int contexts_length = contexts.size();
        for( int i = 0; i < contexts_length; ++i ) {
            result += contexts[i]->indices_length;
        }

        return result / 3;
    }

    /**
     * 描画用マテリアルを取得する
     */
    virtual jc_sp<MaterialType> getMaterial() const {
        return material;
    }

    virtual void setMaterial( jc_sp<MaterialType> material ) {
        this->material = material;
    }
};

}

#endif /* MESHFRAGMENT_H_ */
