/*
 * jcglQuad.cpp
 *
 *  Created on: 2012/10/03
 */

#include    "jc/gl/Quad.h"
#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"

namespace jc {
namespace gl {

namespace {


#define LEFT    -0.5
#define TOP     0.5
#define RIGHT   0.5
#define BOTTOM  -0.5

const static Quad::QuadVertex g_vertices[] = {
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
    this->attrVertices = ATTRIBUTE_DISABLE_INDEX;
    this->attrCoords = ATTRIBUTE_DISABLE_INDEX;
    this->state = device->getState();
    this->vertices.alloc(device->getVRAM(), VRAM_VertexBufferObject);
    this->primitiveType = GL_TRIANGLE_FAN;
    this->initialize();

//    glLineWidth(2);
//    primitiveType = GL_LINE_LOOP;
}

/**
 * 初期化を行う。
 */
void Quad::initialize() {
    jclogf("quad initialize : %x", this);
    jclogf("quad id = %d", vertices.get());

    updateVertices(g_vertices);

}

/**
 * 解放を行う。
 */
Quad::~Quad() {
}

/**
 * 属性インデックスを指定する。
 * ATTRIBUTE_DISABLE_INDEXを指定することで、NULL（無効）にできる。
 */
void Quad::attributes(const s32 attribute_vertices, const s32 attribute_coords) {
    attrVertices = attribute_vertices;
    attrCoords = attribute_coords;
}

/**
 * 描画を行う。
 * レンダリング環境はバインド元に従う。
 */
void Quad::rendering() {
    state->bindBuffer(GL_ARRAY_BUFFER, vertices.get());

    // 頂点バッファ
    if (attrVertices >= 0) {
        state->enableVertexAttribArray(attrVertices);
        state->vertexAttribPointer(attrVertices, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), NULL, 0);
    } else {
        // attrVが存在しないならレンダリングできない。
        return;
    }
    // UVバッファ
    if (attrCoords >= 0) {
        state->enableVertexAttribArray(attrCoords);
        state->vertexAttribPointer(attrCoords, 2, GL_FLOAT, GL_FALSE, sizeof(QuadVertex), NULL, sizeof(float) * 2);
    }
    CLEAR_GL_ERROR();
    glDrawArrays(primitiveType, 0, 4);
    PRINT_GL_ERROR();
}

/**
 * 頂点情報を更新する。
 * 4頂点を設定しなければならない。
 */
void Quad::updateVertices(const QuadVertex *vertices) {
    // ステートの割り当て
    state->bindBuffer(GL_ARRAY_BUFFER, this->vertices.get());

// バッファ転送
    glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertex) * 4, vertices, GL_STATIC_DRAW);
    PRINT_GL_ERROR();
}


}
}
