/*
 * FigureLoader.h
 *
 *  Created on: 2012/12/26
 */

#ifndef FIGUREDATALOADER_H_
#define FIGUREDATALOADER_H_

#include    "jointcoding.h"
#include    "jc/data/BinaryInputStream.h"
#include    "jc/math/Transform.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Vec4.h"
#include    "jc/graphics/figure/Figure.h"

#include    "jc/graphics/figure/data/FigureDataFactory.h"

namespace jc {

/**
 * フィギュアのデータを読み込む。
 */
class FigureDataLoader: public Object {
public:

    /**
     * Figure情報を構築する
     */
    struct FigureInfo {
        /**
         * 全ノード数
         */
        u32 node_num;

        /**
         * 頂点数
         */
        u32 vertex_num;

        /**
         * インデックス数
         */
        u32 index_num;

        /**
         * マテリアル数
         */
        u32 material_num;

        FigureInfo() {
            vertex_num = index_num = material_num = node_num = 0;
        }
    };

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
                TransformRotateOrder_e order;

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
                 * w情報
                 * オイラー角の場合、wは常に0となる。
                 * クォータニオンを利用している場合のみ利用する。
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

    struct MaterialInfo {
        /**
         * マテリアル名
         */
        String name;

        /**
         * テクスチャ名
         */
        String texture_name;

        /**
         *
         */
        Color diffuse;

        /**
         *
         */
        Color ambient;

        /**
         *
         */
        Color emissive;
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
         * 読み込んだマテリアル
         */
        std::vector<MaterialInfo> materials;

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

    /**
     * メッシュを構築するデータ
     */
    struct MeshData {
        /**
         * 読み込んだデータ。
         * fixed -> float以外は基本的に無加工
         */
        jc_sa<u8> data;

        /**
         * データの配列数。
         * バイト数では無いため、バイト数が欲しい場合はsizeof(型) * data_lengthを行う。
         */
        u32 data_length;

        /**
         * データの種類
         */
        MeshDataType_e type;
    };

    FigureDataLoader(MFigureDataFactory factory);
    virtual ~FigureDataLoader();

    /**
     * 読み込みを行う。
     * ファイル名等の初期化は継承したサブクラスで行う。
     */
    virtual void load();

protected:
    /**
     * データファクトリー
     */
    MFigureDataFactory factory;

    /**
     * 読み込むべきフィギュアデータの読み込みが完了した
     */
    virtual void onLoadCompleted() {

    }

    /**
     * 指定した番号のノードを読み込む
     */
    virtual void loadNode( const s32 nodeNumber);

    /**
     * 指定したメッシュ・コンテキストのデータを読み込む
     * 読み込み後、完了メッセージを送る
     */
    virtual void loadMeshData( const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num, const MeshDataType_e type);

    /**
     * フィギュアの基本情報を読み込んだ後に呼び出される
     * サブクラスではノード数分のメモリ確保等を行う。
     */
    virtual void onFigureInfoLoadComplete( const FigureInfo &figureInfo) {

    }

    /**
     * ノード情報を読み込んだ場合に呼び出される
     */
    virtual void onNodeLoadComplete( const NodeInfo &nodeInfo ) {

    }

    /**
     * onNodeLoaded()完了後、メッシュ情報を読み込んだ後に呼び出される
     *
     * @param load_request 読み込みを行うデータのリクエストを行う
     */
    virtual void onMeshInfoLoadComplete( const NodeInfo &nodeInfo, const MeshInfo &meshInfo, MeshDataRequest *load_request ) {

    }

    /**
     * メッシュデータの読み込みを完了した
     *
     * @param material_num マテリアル番号
     * @param context_num コンテキスト番号
     * @param dataType 読み込みを行ったデータタイプ
     * @param loaded_data 読み込みを行ったデータ
     */
    virtual void onMeshDataLoadComplete( const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num, const MeshData &loaded ) {
    }

    /**
     * 指定したコンテキストが全て読み込み終わった。
     * サブクラスではVBO転送を期待する。
     */
    virtual void onMeshMaterialContextLoadComplete(const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num, const s32 context_num) {
    }

    /**
     * メッシュ内の1マテリアルを読み込み終わった。
     * 1マテリアルは複数のコンテキストに分割して描画する必要がある。
     */
    virtual void onMeshMaterialLoadComplete( const NodeInfo &nodeInfo, const MeshInfo &meshInfo, const s32 material_num) {
    }

    /**
     * メッシュデータを読み込み終わった
     */
    virtual void onMeshLoadComplete(const FigureDataLoader::NodeInfo &nodeInfo, const FigureDataLoader::MeshInfo &meshInfo) {
    }
};

}

#endif /* FIGURELOADER_H_ */
