/*
 * JCMatrix4x4.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCMATRIX4X4_H_
#define JCMATRIX4X4_H_

#include    "jc/system/Macro.h"
#include    "jc/math/Math.h"
#include    "jc/math/Vec3.h"
namespace jc {

/**
 * 4x4のOpenGL ES 2.0に最適化された行列クラスを扱う。
 * 3D利用の場合
 *     ROW    = 4
 *     COLMN >= 3
 *
 *               → colmn
 *         1 0 0 0
 *         0 1 0 0
 *     ↓  0 0 1 0
 * row     0 0 0 1
 */
template<s32 ROW, s32 COLM>
struct Matrix {
    float m[ROW][COLM];

    Matrix() {
        identity();
    }

    /**
     * 単位行列に設定する
     */
    inline void identity() {
        for (int i = 0; i < ROW; ++i) {
            for (int k = 0; k < COLM; ++k) {
                if (i == k) {
                    m[i][k] = 1.0f;
                } else {
                    m[i][k] = 0;
                }
            }
        }
    }

    /**
     * ベクトルと行列の掛け算を行う
     * result == &originでも問題なく動作する
     */
    inline Vector3f* multiply(const Vector3f &origin, Vector3f *result) {
        result->set(
//
                // x
                m[0][0] * origin.x + m[1][0] * origin.y + m[2][0] * origin.z + m[3][0],
                // y
                m[0][1] * origin.x + m[1][1] * origin.y + m[2][1] * origin.z + m[3][1],
                // z
                m[0][2] * origin.x + m[1][2] * origin.y + m[2][2] * origin.z + m[3][2]);
        return result;
    }

    /**
     * 位置行列を生成する
     */
    inline void translate(const float x, const float y, const float z) {
        m[3][0] = x;
        m[3][1] = y;
        m[3][2] = z;
    }

    /**
     * 回転行列を生成する
     *
     * XYZはそれぞれ回転軸、Wが回転量を示す。
     */
    inline void rotate(const float x, const float y, const float z, const float degree) {
        const float _cos = ::cos(degree2radian(degree));
        const float _sin = ::sin(degree2radian(degree));

        const float Xsin = x * _sin;
        const float Ysin = y * _sin;
        const float Zsin = z * _sin;

        const float XX = x * x;
        const float YY = y * y;
        const float ZZ = z * z;

        const float XY = x * y;
        const float XZ = x * z;
        const float YZ = y * z;

        {
            m[0][0] = (XX * (1.0f - _cos)) + _cos;
            m[0][1] = (XY * (1.0f - _cos)) - Zsin;
            m[0][2] = (XZ * (1.0f - _cos)) + Ysin;
        }
        {
            m[1][0] = (XY * (1.0f - _cos)) - Zsin;
            m[1][1] = (YY * (1.0f - _cos)) + _cos;
            m[1][2] = (YZ * (1.0f - _cos)) - Xsin;
        }
        {
            m[2][0] = (XZ * (1.0f - _cos)) - Ysin;
            m[2][1] = (YZ * (1.0f - _cos)) + Xsin;
            m[2][2] = (XY * (1.0f - _cos)) + _cos;
        }
    }

    /**
     * 拡大縮小行列
     */
    inline void scale(const float x, const float y, const float z) {
        m[0][0] = x;
        m[1][1] = y;
        m[2][2] = z;
    }

    /**
     *
     */
    inline float* operator[](const int row) {
        return m[row];
    }

    inline float get(const int row, const int colmn) {
        return m[row][colmn];
    }

    /**
     * 内容をデバッグ出力する
     */
    inline void print() {
        for (int r = 0; r < ROW; ++r) {
            switch (COLM) {
                case 3:
                    jclogf("  %d | %05.3f, %05.3f, %05.3f", r, m[r][0], m[r][1], m[r][2]);
                    break;
                case 4:
                    jclogf("  %d | %05.3f, %05.3f, %05.3f, %05.3f", r, m[r][0], m[r][1], m[r][2], m[r][3]);
                    break;
            }
        }
    }
};

}
#endif /* JCMATRIX4X4_H_ */
