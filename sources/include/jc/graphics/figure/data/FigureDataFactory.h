/*
 * FigureDataFactory.h
 *
 *  Created on: 2013/01/08
 */

#ifndef FIGUREDATAFACTORY_H_
#define FIGUREDATAFACTORY_H_

#include    "jointcoding.h"
#include    "jc/data/BinaryInputStream.h"

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
 * アニメーションデータタイプを定義する
 */
enum AnimationDataType_e {
    /**
     * 平行移動
     */
    AnimationDataType_Translate,

    /**
     * 回転角
     */
    AnimationDataType_Rotate,

    /**
     * スケーリング値
     */
    AnimationDataType_Scale,
};

class FigureDataFactory: public Object {
public:
    FigureDataFactory() {
    }

    virtual ~FigureDataFactory() {
    }

    /**
     * 大本のインフォメーションを取得する
     */
    virtual MBinaryInputStream openFigureInfo() = 0;

    /**
     * ノードを読み込む
     */
    virtual MBinaryInputStream openNode(const s32 nodeNumber) = 0;

    /**
     * 指定したノードのメッシュ情報を読み込む
     */
    virtual MBinaryInputStream openMeshInfo(const s32 nodeNumber) = 0;

    /*
     * 指定したマテリアル情報を読み込む
     */
    virtual MBinaryInputStream openMaterialInfo(const s32 nodeNumber, const s32 materialNumber) = 0;

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

    /**
     * アニメーション用データを開く
     */
    virtual MBinaryInputStream openAnimationData(const AnimationDataType_e type, const u32 clipNumber, const u32 nodeNumber) = 0;
};

/**
 * managed
 */
typedef jc_sp<FigureDataFactory> MFigureDataFactory;

}

#endif /* FIGUREDATAFACTORY_H_ */
