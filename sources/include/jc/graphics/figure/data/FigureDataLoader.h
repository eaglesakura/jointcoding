/*
 * FigureLoader.h
 *
 *  Created on: 2012/12/26
 */

#ifndef FIGURELOADER_H_
#define FIGURELOADER_H_

#include    "jointcoding.h"
#include    "jc/data/BinaryInputStream.h"
#include    "jc/math/EulerTransform.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Vec4.h"
#include    "jc/graphics/figure/Figure.h"

namespace jc {

enum MeshDataType_e {
    /**
     * インデックスバッファ
     *
     * u16[]
     */
    MeshDataType_Indices,

    /**
     * 位置情報
     *
     * Vector3f[]
     */
    MeshDataType_Positions,

    /**
     * UV情報
     *
     * Vector2f[]
     */
    MeshDataType_Coords,

    /**
     * 法線情報
     *
     * Vector3f[]
     */
    MeshDataType_Normals,

    /**
     * ボーンのピックアップテーブル
     */
    MeshDataType_BonePickTables,

    /**
     * ボーンのインデックス
     * MeshDataType_BonePickTables[ MeshDataType_BoneIndices ]にアクセスすることで実際のボーン行列を得る
     */
    MeshDataType_BoneIndices,

    /**
     * ボーンの重み情報
     */
    MeshDataType_BoneWeight,
};

/**
 * フィギュアのデータを読み込む。
 */
class FigureDataLoader: public Object {
public:

    /**
     * ノード情報を構築する
     */
    struct NodeInfo {
        /**
         * ノード名
         */
        String name;

        /**
         * ノード番号
         */
        s32 number;

        /**
         * ノードタイプ
         */
        NodeType_e type;

        /**
         * 子ノード数
         */
        u32 children_num;

        /**
         * 子ノードのID一覧
         */
        jc_sa<s32> children;

        /**
         * デフォルトの姿勢情報
         */
        struct {
            /**
             * スケーリング情報
             */
            Vector3f scale;

            /**
             * 回転情報
             */
            struct {

                /**
                 * 回転のオーダー
                 */
                EulerRotateOrder_e order;

                /**
                 * x
                 */
                float x;

                /**
                 * y
                 */
                float y;

                /**
                 * z
                 */
                float z;

                /**
                 * W情報
                 */
                float w;
            }rotate;

            /**
             * 位置情報
             */
            Vector3f translate;
        }def_transform;
        NodeInfo() {
            type = NodeType_NULL;
            number = children_num = 0;
            def_transform.rotate.x = def_transform.rotate.y = def_transform.rotate.z = def_transform.rotate.w = 0;
        }
    };

    /**
     * メッシュ情報を格納する
     */
    struct MeshInfo {
        /**
         * マテリアル数
         */
        u32 material_num;

        /**
         * 各マテリアルごとのコンテキスト数
         */
        jc_sa<u32> context_num;

        MeshInfo() {
            material_num = 0;
        }
    };

    /**
     * メッシュに必要なオプションデータをリクエストする
     */
    struct MeshDataRequest {
        /**
         * UV情報
         */
        jcboolean coords;

        /**
         * 法線情報
         */
        jcboolean normals;

        /**
         * ボーンの重み情報
         * * PickTable
         * * Weight
         * * WeightIndex
         */
        jcboolean bone_weight;

        MeshDataRequest() {
            coords = jcfalse;
            normals = jcfalse;
            bone_weight = jcfalse;
        }
    };

    FigureDataLoader();
    virtual ~FigureDataLoader();

    /**
     * 読み込みを行う。
     * ファイル名等の初期化は継承したサブクラスで行う。
     */
    virtual void load();

protected:
    /**
     * 指定した番号のノードを読み込む
     */
    virtual void loadNode( const s32 nodeNumber);

    /**
     * ノード情報を読み込んだ場合に呼び出される
     */
    virtual void onNodeLoadComplete( const NodeInfo &nodeInfo ) = 0;

    /**
     * onNodeLoaded()完了後、メッシュ情報を読み込んだ後に呼び出される
     *
     * @param load_request 読み込みを行うデータのリクエストを行う
     */
    virtual void onMeshLoadComplete( const NodeInfo &nodeInfo, const MeshInfo &meshInfo, MeshDataRequest *load_request ) = 0;

    /**
     * メッシュデータの読み込みを完了した
     *
     * @param material_num マテリアル番号
     * @param context_num コンテキスト番号
     * @param dataType 読み込みを行ったデータタイプ
     * @param loaded_data 読み込みを行ったデータ
     */
    virtual void onMeshDataLoadComplete( const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num, const MeshDataType_e dataType, jc_sa<void*> loaded_data ) = 0;

    /**
     * ノードを読み込む
     */
    virtual MBinaryInputStream openNode(const s32 nodeNumber) = 0;

    /**
     * 指定したノードのメッシュ情報を読み込む
     */
    virtual MBinaryInputStream openMeshInfo(const s32 nodeNumber) = 0;

    /**
     * メッシュデータを読み込む。
     *
     * 各ノードはマテリアルごと、複数のコンテキストにわかれている。
     * 複数のコンテキストを組み上げることで１つのマテリアル分の頂点データが出来上がる。
     * １メッシュで大量のボーンを使った場合、ベクタユニット数が足りなくなることを防ぐため。
     *
     * @param type 得たい情報のタイプ（拡張子に対応する）
     * @param nodeNumber ノード番号
     * @param materialNumber マテリアル番号
     * @param contextNumber コンテキスト番号
     */
    virtual MBinaryInputStream openMeshData(const MeshDataType_e type, const s32 nodeNumber, const u32 materialNumber, const u32 contextNumber) = 0;
};

}

#endif /* FIGURELOADER_H_ */
