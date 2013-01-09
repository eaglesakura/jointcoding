/*
 * AssetFigureDataFactory.cpp
 *
 *  Created on: 2013/01/08
 */

#include    "jc/graphics/figure/data/UriFigureDataFactory.h"
#include    "jc/platform/Platform.h"

#ifdef BUILD_Android

namespace jc {

/**
 * 大本のインフォメーションを取得する
 */
MBinaryInputStream UriFigureDataFactory::openFigureInfo() {
    Uri uri = Uri::parse(String::format("%s/figure.info", baseUri.getUri().c_str()));
    return MBinaryInputStream(new BinaryInputStream(Platform::getFileSystem()->openInputStream(uri)));
}

/**
 * ノードを読み込む
 */
MBinaryInputStream UriFigureDataFactory::openNode(const s32 nodeNumber) {
    Uri uri = Uri::parse(String::format("%s/node%03d.node", baseUri.getUri().c_str(), nodeNumber));
    return MBinaryInputStream(new BinaryInputStream(Platform::getFileSystem()->openInputStream(uri)));
}

/**
 * 指定したノードのメッシュ情報を読み込む
 */
MBinaryInputStream UriFigureDataFactory::openMeshInfo(const s32 nodeNumber) {
    Uri uri = Uri::parse(String::format("%s/node%03d.meshinfo", baseUri.getUri().c_str(), nodeNumber));
    return MBinaryInputStream(new BinaryInputStream(Platform::getFileSystem()->openInputStream(uri)));
}

/*
 * 指定したマテリアル情報を読み込む
 */
MBinaryInputStream UriFigureDataFactory::openMaterialInfo(const s32 nodeNumber, const s32 materialNumber) {
    Uri uri = Uri::parse(String::format("%s/node%03d_mtl%02d.material", baseUri.getUri().c_str(), nodeNumber, materialNumber));
    return MBinaryInputStream(new BinaryInputStream(Platform::getFileSystem()->openInputStream(uri)));
}

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
MBinaryInputStream UriFigureDataFactory::openMeshData(const MeshDataType_e type, const s32 nodeNumber, const u32 materialNumber, const u32 contextNumber) {

#if 0

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
#endif
    const char *ext_table[] = { "indices", "vertices", "coords", "normals", "bones", "weightindices", "weight" };

    Uri uri = Uri::parse(String::format("%s/node%03d_mtl%02d_ctx%02d.%s", baseUri.getUri().c_str(), nodeNumber, materialNumber, contextNumber, ext_table[type]));
    return MBinaryInputStream(new BinaryInputStream(Platform::getFileSystem()->openInputStream(uri)));
}

/**
 * アニメーション用データを開く
 */
MBinaryInputStream UriFigureDataFactory::openAnimationData(const AnimationDataType_e type, const u32 clipNumber, const u32 nodeNumber) {
#if  0

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
#endif
    const char *ext_table[] = { "t", "r", "s" };

    Uri uri = Uri::parse(String::format("%s/node%03d.anim_%s", baseUri.getUri().c_str(), nodeNumber, ext_table[type]));
    return MBinaryInputStream(new BinaryInputStream(Platform::getFileSystem()->openInputStream(uri)));
}

}

#endif
