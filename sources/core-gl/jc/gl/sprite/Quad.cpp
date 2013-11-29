/*
 * jcglQuad.cpp
 *
 *  Created on: 2012/10/03
 */

#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"
#include    "jc/gl/sprite/Quad.h"

namespace jc {
namespace gl {

namespace {

#define LEFT    -0.5
#define TOP     0.5
#define RIGHT   0.5
#define BOTTOM  -0.5

const static QuadVertex g_vertices[] = {
//
        /**
         // 位置情報
         left, top, //!< 左上
         right, top, //!< 右上
         left, bottom, //!< 左下
         right, bottom, //!< 右下

         //! UV情報
         0, 0, //!< 左上
         1, 0, //!< 右上
         0, 1, //!< 左下
         1, 1, //!< 右下
         */
        // 左上
        { LEFT, TOP, 0.0f, 0.0f, },
        // 左下
        { LEFT, BOTTOM, 0.0f, 1.0f },
        // 右下
        { RIGHT, BOTTOM, 1.0f, 1.0f },
        // 右上
        { RIGHT, TOP, 1.0f, 0.0f },
// end
        };
}

#undef  LEFT
#undef  TOP
#undef  RIGHT
#undef  BOTTOM

/**
 * 矩形を構築する
 */
Quad::Quad(MDevice device) {
    this->vertices.alloc(device->getVRAM(VRAM_VertexBufferObject));
    assert(vertices);
    this->primitiveType = GL_TRIANGLE_FAN;
    this->initialize(device->getState());

//    glLineWidth(2);
//    primitiveType = GL_LINE_LOOP;
}

/**
 * 初期化を行う。
 */
void Quad::initialize(MGLState state) {
    jclogf("quad initialize : %x", this);
    jclogf("quad id = %d", vertices.get());

    updateVertices(state, g_vertices);

}

/**
 * 解放を行う。
 */
Quad::~Quad() {
}

/**
 * 描画を行う。
 * レンダリング環境はバインド元に従う。
 */
void Quad::rendering(MGLState state) {
    state->bindBuffer(GL_ARRAY_BUFFER, vertices.get());

    // 位置属性が存在しないならレンダリングできない
    if (!attr.pos.valid()) {
        return;
    }

    // 頂点バッファ
    attr.pos.attributePointer(state);
    attr.coord.attributePointer(state);

    glDrawArrays(primitiveType, 0, 4);
    assert_gl();
}

/**
 * 頂点情報を更新する。
 * 4頂点を設定しなければならない。
 */
void Quad::updateVertices(MGLState state, const QuadVertex *vertices) {
    // ステートの割り当て
    state->bindBuffer(GL_ARRAY_BUFFER, this->vertices.get());

// バッファ転送
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * 4, vertices, GL_STATIC_DRAW);
    assert_gl();
}

}
}
