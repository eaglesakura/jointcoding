/*
 * FigureDataOutputStream.h
 *
 *  Created on: 2012/12/25
 */

#ifndef FIGUREDATAOUTPUTSTREAM_H_
#define FIGUREDATAOUTPUTSTREAM_H_

#include    "jointcoding.h"
#include    "jc/math/Matrix.h"
#include    "jc/math/Vec2.h"
#include    "jc/data/BinaryOutputStream.h"

namespace jc {

class FigureDataOutputStream: public BinaryOutputStream {

public:
    enum ModelerType_e {
        /**
         * 3dsMaxで出力した
         */
        ModelerType_3dsMax,

        /**
         * 不明
         */
        ModelerType_Unknown,
    };
protected:
    /**
     * モデラーの種類
     * 座標系が違うため、補正を行う
     */
    ModelerType_e modeler;

    /**
     * 事前計算を行なっておくための行列
     */
    Matrix4x3 modelMatrix;

    /**
     * 固定小数ビット長
     * デフォルトはGL長
     */
    u32 fixedbits;
public:

    FigureDataOutputStream(MOutputStream stream);
    virtual ~FigureDataOutputStream();

    /**
     * モデラーを設定する
     */
    virtual void initialize(const ModelerType_e modeler);

    /**
     * 頂点を書き出す
     * モデラー補正を行う
     *
     * @param v 書き出す頂点座標
     * @param transform 頂点の位置を補正する場合true
     */
    virtual void writeVector3(const Vector3f &v, const jcboolean transform = jctrue);

    /**
     * 頂点配列を直接吐き出す。
     * Vertex構造体を直接渡せるように、GLと同じく隙間を設定できる
     */
    virtual void writeVertices(const void* vertices, const u32 vertices_length, const u32 sizeof_once_vertex, const jcboolean transform = jctrue);

    /**
     * UV座標を書き出す
     *
     */
    virtual void writeCoord(const Vector2f &v);
};

/**
 * managed
 */
typedef jc_sp<FigureDataOutputStream> MFigureDataOutputStream;

}

#endif /* FIGUREDATAOUTPUTSTREAM_H_ */
