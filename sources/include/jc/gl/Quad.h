/*
 * jcglQuad.h
 *
 *  Created on: 2012/10/03
 */

#ifndef JCGLQUAD_H_
#define JCGLQUAD_H_

#include    "jc/gl/GL.h"
#include    "jc/gl/Device.h"

namespace jc {
namespace gl {

/**
 * 矩形ポリゴンを管理する。
 * left,top / right top <BR>
 * ------------------- <BR>
 * |　　　　　　　　　　| <BR>
 * |　　　　　　　　　　| <BR>
 * |　　　　　　　　　　| <BR>
 * |　　　　　　　　　　| <BR>
 * ------------------- <BR>
 * left bottom / right bottom <BR>
 *
 *
 * データは左上、右上、左下、右下の順番に格納される。
 */
class Quad: public Object {
    /**
     * 保存するステート
     */
    MGLState state;

    /**
     * 頂点バッファ
     */
    SharedResource vertices;

    /**
     * 頂点バッファの属性インデックス
     */
    s32 attrVertices;

    /**
     * UVバッファの属性インデックス
     */
    s32 attrCoords;

    /**
     * 初期化を行う
     */
    void initialize();

public:
    /**
     * 矩形を構築する
     */
    Quad(MDevice device);

    /**
     * 解放を行う。
     */
    virtual ~Quad();

    /**
     * 属性インデックスを指定する。
     * ATTRIBUTE_DISABLE_INDEXを指定することで、NULL（無効）にできる。
     */
    virtual void attributes(const s32 attribute_vertices, const s32 attribute_coords);

    /**
     * 描画を行う。
     * レンダリング環境はバインド元に従う。
     */
    virtual void rendering();
};

}
}

#endif /* JCGLQUAD_H_ */
