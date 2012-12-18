/*
 * Vetex.h
 *
 *  Created on: 2012/12/18
 */

#ifndef VETEX_H_
#define VETEX_H_

#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"

namespace jc {

/**
 * Figure向けに構築する汎用的な頂点情報
 */
struct FigureVetex {
    /**
     * 位置情報
     */
    Vector3f position;

    /**
     * UV情報
     */
    Vector2f uv;

    /**
     * 頂点インデックス
     */
    s8 indices[4];

    /**
     * 頂点ウェイト
     */
    float weight[4];
};

}

#endif /* VETEX_H_ */
