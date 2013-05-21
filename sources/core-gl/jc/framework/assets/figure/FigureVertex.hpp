/*
 * FigureVertex.hpp
 *
 *  Created on: 2013/05/20
 */

#ifndef FIGUREVERTEX_HPP_
#define FIGUREVERTEX_HPP_

#include    "jointcoding.h"
#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Vec4.h"
#include    "jc/gl/GL.h"

namespace jc {
namespace fw {

/**
 * スキニング情報を含んだ頂点バッファ
 *
 * ボーンウェイトはシェーダーに転送するときにNORMALIZEをかけることでfloat化する。
 */
struct SkinVertex {
    /**
     * 位置情報
     */
    Vector3f position;

    /**
     * 頂点のUV
     */
    Vector2f coord;

    /**
     * 法線
     */
    Vector3f normal;

    /**
     * ボーンのインデックス
     */
    GLubyte bone_indices[4];

    /**
     * ボーンの重み情報
     */
    GLubyte bone_weights[4];
};

}
}

#endif /* FIGUREVERTEX_HPP_ */
