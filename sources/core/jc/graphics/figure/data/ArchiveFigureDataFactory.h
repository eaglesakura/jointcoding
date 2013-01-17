/*
 * ArchiveFigureDataFactory.h
 *
 *  Created on: 2013/01/17
 */

#ifndef ARCHIVEFIGUREDATAFACTORY_H_
#define ARCHIVEFIGUREDATAFACTORY_H_

#include    "jc/graphics/figure/data/FigureDataFactory.h"

namespace jc {

class ArchiveFigureDataFactory: public FigureDataFactory {

public:
    /**
     * 大本のインフォメーションを取得する
     */
    virtual MBinaryInputStream openFigureInfo();

    /**
     * ノードを読み込む
     */
    virtual MBinaryInputStream openNode(const s32 nodeNumber);

    /**
     * 指定したノードのメッシュ情報を読み込む
     */
    virtual MBinaryInputStream openMeshInfo(const s32 nodeNumber);

    /*
     * 指定したマテリアル情報を読み込む
     */
    virtual MBinaryInputStream openMaterialInfo(const s32 nodeNumber, const s32 materialNumber);

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
    virtual MBinaryInputStream openMeshData(const MeshDataType_e type, const s32 nodeNumber, const u32 materialNumber, const u32 contextNumber);

    /**
     * アニメーション用データを開く
     */
    virtual MBinaryInputStream openAnimationData(const AnimationDataType_e type, const u32 clipNumber, const u32 nodeNumber);
};

}

#endif /* ARCHIVEFIGUREDATAFACTORY_H_ */
