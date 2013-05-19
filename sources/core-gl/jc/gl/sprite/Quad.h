/*
 * jcglQuad.h
 *
 *  Created on: 2012/10/03
 */

#ifndef JCGLQUAD_H_
#define JCGLQUAD_H_

#include    "jc/gl/GL.h"
#include    "jc/math/Vec2.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/gl/shader/VertexAttribute.hpp"

namespace jc {
namespace gl {

/**
 * 四角形構築用の頂点構造体
 */
struct QuadVertex {
    float x;
    float y;

    float u;
    float v;
};

/**
 * 位置属性
 */
typedef VertexAttribute<QuadVertex, 2, GL_FLOAT, GL_FALSE, 0> QuadPositionAttribute;

/**
 * UV属性
 */
typedef VertexAttribute<QuadVertex, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2f)> QuadCoordAttribute;

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
    vram_handle vertices;

    struct {
        /**
         * 位置情報
         */
        QuadPositionAttribute pos;

        /**
         * UV情報
         */
        QuadCoordAttribute coord;
    } attr;

    /**
     * 初期化を行う
     */
    void initialize();

    /**
     * レンダリングモード
     */
    GLenum primitiveType;
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
     *
     */
    virtual void setPositionAttribute(const QuadPositionAttribute &attr) {
        this->attr.pos = attr;
    }

    /**
     *
     */
    virtual void setCoordAttribute(const QuadCoordAttribute &attr) {
        this->attr.coord = attr;
    }

    /**
     * 描画するプリミティブの種類を変更する
     */
    virtual void setPrimitiveType(const GLenum type) {
        this->primitiveType = type;
    }

    /**
     * 描画を行う。
     * レンダリング環境はバインド元に従う。
     */
    virtual void rendering();

    /**
     * 頂点情報を更新する。
     * 4頂点を設定しなければならない。
     */
    virtual void updateVertices(const QuadVertex *vertices);

};

}
}

#endif /* JCGLQUAD_H_ */
