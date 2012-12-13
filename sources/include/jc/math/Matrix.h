/*
 * JCMatrix4x4.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCMATRIX4X4_H_
#define JCMATRIX4X4_H_

#include    "jc/system/Macro.h"
namespace jc {

/**
 * 4x4のOpenGL ES 2.0に最適化された行列クラスを扱う。
 */
struct Matrix {
    float m[4][4];

    Matrix() {
        identity();
    }

    /**
     * 単位行列に設定する
     */
    inline void identity() {
        for (int i = 0; i < 4; ++i) {
            for (int k = 0; k < 4; ++k) {
                if (i == k) {
                    m[i][k] = 1.0f;
                } else {
                    m[i][k] = 0;
                }
            }
        }
    }

    /**
     *
     */
    inline float* operator[](s32 index ) {
        return m[index];
    }
};

}
#endif /* JCMATRIX4X4_H_ */
