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
#include    "jc/mem/MemoryUtil.h"

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
template<s32 ROW, s32 COLM, typename T = float>
struct Matrix {
    T m[ROW][COLM];

    Matrix() {
        identity();
    }

    Matrix(const Matrix<ROW, COLM, T> &origin) {
        memcpy((void*) m, (const void*) origin.m, sizeof(m));
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
     * 単位行列であることを確認する
     */
    inline jcboolean isIdentity(const float threshold = 0.001f) const {
        for (int i = 0; i < ROW; ++i) {
            for (int k = 0; k < COLM; ++k) {
                if (i == k) {

                    if (!equals(1.0f, m[i][k], threshold)) {
                        jclogf("m[%d][%d] = %f", i, k, m[i][k]);
                        return jcfalse;
                    }

                } else {

                    if (!equals(0.0f, m[i][k], threshold)) {
                        jclogf("m[%d][%d] = %f", i, k, m[i][k]);
                        return jcfalse;
                    }

                }
            }
        }

        return jctrue;
    }

    /**
     * ベクトルと行列の掛け算を行う
     * result == &originでも問題なく動作する
     *
     * COLMNが4以上ある場合、Wの正規化を行う
     */
    inline Vector3f* multiply(const Vector3f &origin, Vector3f *result) const {
        result->set(
//
                // x
                m[0][0] * origin.x + m[1][0] * origin.y + m[2][0] * origin.z + m[3][0],
                // y
                m[0][1] * origin.x + m[1][1] * origin.y + m[2][1] * origin.z + m[3][1],
                // z
                m[0][2] * origin.x + m[1][2] * origin.y + m[2][2] * origin.z + m[3][2]);

        if (COLM >= 4) {
            const float w = m[0][3] * origin.x + m[1][3] * origin.y + m[2][3] * origin.z + m[3][3];
            result->x /= w;
            result->y /= w;
            result->z /= w;
        }

        return result;
    }

    /**
     * float[3]と行列の演算を行う
     */
    inline void multiply3(const float *origin3f, float w, float *result4f) const {

        // x
        result4f[0] = m[0][0] * origin3f[0] + m[1][0] * origin3f[1] + m[2][0] * origin3f[2] + m[3][0] * w;
        // y
        result4f[1] = m[0][1] * origin3f[0] + m[1][1] * origin3f[1] + m[2][1] * origin3f[2] + m[3][1] * w;
        // z
        result4f[2] = m[0][2] * origin3f[0] + m[1][2] * origin3f[1] + m[2][2] * origin3f[2] + m[3][2] * w;
    }

    /**
     * float[3]と行列の演算を行う
     */
    inline void multiply4(const float *origin4f, float *result4f) const {
        // x
        result4f[0] = m[0][0] * origin4f[0] + m[1][0] * origin4f[1] + m[2][0] * origin4f[2] + m[3][0] * origin4f[3];
        // y
        result4f[1] = m[0][1] * origin4f[0] + m[1][1] * origin4f[1] + m[2][1] * origin4f[2] + m[3][1] * origin4f[3];
        // z
        result4f[2] = m[0][2] * origin4f[0] + m[1][2] * origin4f[1] + m[2][2] * origin4f[2] + m[3][2] * origin4f[3];
        // w
        result4f[3] = m[0][3] * origin4f[0] + m[1][3] * origin4f[1] + m[2][3] * origin4f[2] + m[3][3] * origin4f[3];
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
            m[2][2] = (ZZ * (1.0f - _cos)) + _cos;
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

    inline float get(const int row, const int colmn) const {
        return m[row][colmn];
    }

    /**
     * 内容をデバッグ出力する
     */
    inline void print() const {
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

    /**
     * 視線変更行列を生成する。
     * @param position 視線の起点位置
     * @param look 視線の向かう位置
     * @param up 上方向
     *
     * 3D行列のみ利用可能
     */
    inline void lookAt(const Vector3f position, const Vector3f look, const Vector3f up) {
        Vector3f zaxis, xaxis, yaxis;

        zaxis = position - look;
        zaxis.normalize();
        up.cross(zaxis, &xaxis);
        xaxis.normalize();
        zaxis.cross(xaxis, &yaxis);

        {
            m[0][0] = xaxis.x;
            m[1][0] = xaxis.y;
            m[2][0] = xaxis.z;
            m[3][0] = -xaxis.dot(position);

        }
        {

            m[0][1] = yaxis.x;
            m[1][1] = yaxis.y;
            m[2][1] = yaxis.z;
            m[3][1] = -yaxis.dot(position);
        }
        {

            m[0][2] = -zaxis.x;
            m[1][2] = -zaxis.y;
            m[2][2] = -zaxis.z;
            m[3][2] = zaxis.dot(position);
        }

        // 一番右は持っている場合だけ
        if (COLM == 4) {
            m[0][3] = 0;
            m[1][3] = 0;
            m[2][3] = 0;
            m[3][3] = 1;
        }
    }

    /**
     * 射影行列を作成する。
     *
     *
     * @param near
     * @param far
     * @param fovY
     * @param aspect
     *
     */
    void projection(const float near, const float far, const float fovY, const float aspect) {
        float h, w, Q;

        float width_fov = (fovY * (aspect) / 360.0f), height_fov = (fovY / 360.0f);

        w = (float) (1.0 / tan(width_fov * 0.5) / (PI * 2)); // 1/tan(x)
        // ==
        // cot(x)
        h = (float) (1.0 / tan(height_fov * 0.5) / (PI * 2)); // 1/tan(x)
        // ==
        // cot(x)
        Q = far / (far - near);

        // clear
        zeromemory(m, sizeof(m));

        m[0][0] = w;
        m[1][1] = h;
        m[2][2] = Q;
        m[3][2] = -Q * near;
        m[2][3] = 1;
    }

    /**
     * 行列をthisへコピーする
     */
    inline void copy(const Matrix<4, 4> &origin) {
        for (int r = 0; r < ROW; ++r) {
            for (int c = 0; c < min(COLM, 4); ++c) {
                m[r][c] = origin.m[r][c];
            }
        }
    }

    /**
     * 行列をthisへコピーする
     */
    inline void copy(const Matrix<4, 3> &origin) {
        for (int r = 0; r < ROW; ++r) {
            for (int c = 0; c < min(COLM, 3); ++c) {
                m[r][c] = origin.m[r][c];
            }
        }
    }

private:
    /*!
     * 4x4行列の行列式の計算
     *  | m[0]  m[1]  m[2]  m[3]  |
     *  | m[4]  m[5]  m[6]  m[7]  |
     *  | m[8]  m[9]  m[10] m[11] |
     *  | m[12] m[13] m[14] m[15] |
     * @param[in] m 元の行列
     * @return 行列式の値
     */
    template<typename ORIGIN_TYPE>
    static inline double calcDetMat4x4(const ORIGIN_TYPE* m) {
        return m[0] * m[5] * m[10] * m[15] + m[0] * m[6] * m[11] * m[13] + m[0] * m[7] * m[9] * m[14] + m[1] * m[4] * m[11] * m[14] + m[1] * m[6] * m[8] * m[15] + m[1] * m[7] * m[10] * m[12] + m[2] * m[4] * m[9] * m[15] + m[2] * m[5] * m[11] * m[12] + m[2] * m[7] * m[8] * m[13] + m[3] * m[4] * m[10] * m[13] + m[3] * m[5] * m[8] * m[14] + m[3] * m[6] * m[9] * m[12] - m[0] * m[5] * m[11] * m[14]
                - m[0] * m[6] * m[9] * m[15] - m[0] * m[7] * m[10] * m[13] - m[1] * m[4] * m[10] * m[15] - m[1] * m[6] * m[11] * m[12] - m[1] * m[7] * m[8] * m[14] - m[2] * m[4] * m[11] * m[13] - m[2] * m[5] * m[8] * m[15] - m[2] * m[7] * m[9] * m[12] - m[3] * m[4] * m[9] * m[14] - m[3] * m[5] * m[10] * m[12] - m[3] * m[6] * m[8] * m[13];
    }

    /*!
     * 4x4行列の行列式の計算.
     * inputされる行列は必ず4x4としてコピーされた状態で作成される。
     *  | m[0]  m[1]  m[2]  m[3]  |
     *  | m[4]  m[5]  m[6]  m[7]  |
     *  | m[8]  m[9]  m[10] m[11] |
     *  | m[12] m[13] m[14] m[15] |
     * @param[in] m 元の行列
     * @param[out] invm 逆行列
     * @return 逆行列の存在
     */
    template<typename ORIGIN_TYPE>
    inline void _invert(const ORIGIN_TYPE *m, T *invm) const {
        double det = calcDetMat4x4<ORIGIN_TYPE>(m);

        double inv_det = 1.0 / det;

        int index = 0;
        if (COLM == 4) {

            invm[index++] = (T) (inv_det * (m[5] * m[10] * m[15] + m[6] * m[11] * m[13] + m[7] * m[9] * m[14] - m[5] * m[11] * m[14] - m[6] * m[9] * m[15] - m[7] * m[10] * m[13]));
            invm[index++] = (T) (inv_det * (m[1] * m[11] * m[14] + m[2] * m[9] * m[15] + m[3] * m[10] * m[13] - m[1] * m[10] * m[15] - m[2] * m[11] * m[13] - m[3] * m[9] * m[14]));
            invm[index++] = (T) (inv_det * (m[1] * m[6] * m[15] + m[2] * m[7] * m[13] + m[3] * m[5] * m[14] - m[1] * m[7] * m[14] - m[2] * m[5] * m[15] - m[3] * m[6] * m[13]));
            invm[index++] = (T) (inv_det * (m[1] * m[7] * m[10] + m[2] * m[5] * m[11] + m[3] * m[6] * m[9] - m[1] * m[6] * m[11] - m[2] * m[7] * m[9] - m[3] * m[5] * m[10]));

            invm[index++] = (T) (inv_det * (m[4] * m[11] * m[14] + m[6] * m[8] * m[15] + m[7] * m[10] * m[12] - m[4] * m[10] * m[15] - m[6] * m[11] * m[12] - m[7] * m[8] * m[14]));
            invm[index++] = (T) (inv_det * (m[0] * m[10] * m[15] + m[2] * m[11] * m[12] + m[3] * m[8] * m[14] - m[0] * m[11] * m[14] - m[2] * m[8] * m[15] - m[3] * m[10] * m[12]));
            invm[index++] = (T) (inv_det * (m[0] * m[7] * m[14] + m[2] * m[4] * m[15] + m[3] * m[6] * m[12] - m[0] * m[6] * m[15] - m[2] * m[7] * m[12] - m[3] * m[4] * m[14]));
            invm[index++] = (T) (inv_det * (m[0] * m[6] * m[11] + m[2] * m[7] * m[8] + m[3] * m[4] * m[10] - m[0] * m[7] * m[10] - m[2] * m[4] * m[11] - m[3] * m[6] * m[8]));

            invm[index++] = (T) (inv_det * (m[4] * m[9] * m[15] + m[5] * m[11] * m[12] + m[7] * m[8] * m[13] - m[4] * m[11] * m[13] - m[5] * m[8] * m[15] - m[7] * m[9] * m[12]));
            invm[index++] = (T) (inv_det * (m[0] * m[11] * m[13] + m[1] * m[8] * m[15] + m[3] * m[9] * m[12] - m[0] * m[9] * m[15] - m[1] * m[11] * m[12] - m[3] * m[8] * m[13]));
            invm[index++] = (T) (inv_det * (m[0] * m[5] * m[15] + m[1] * m[7] * m[12] + m[3] * m[4] * m[13] - m[0] * m[7] * m[13] - m[1] * m[4] * m[15] - m[3] * m[5] * m[12]));
            invm[index++] = (T) (inv_det * (m[0] * m[7] * m[9] + m[1] * m[4] * m[11] + m[3] * m[5] * m[8] - m[0] * m[5] * m[11] - m[1] * m[7] * m[8] - m[3] * m[4] * m[9]));

            invm[index++] = (T) (inv_det * (m[4] * m[10] * m[13] + m[5] * m[8] * m[14] + m[6] * m[9] * m[12] - m[4] * m[9] * m[14] - m[5] * m[10] * m[12] - m[6] * m[8] * m[13]));
            invm[index++] = (T) (inv_det * (m[0] * m[9] * m[14] + m[1] * m[10] * m[12] + m[2] * m[8] * m[13] - m[0] * m[10] * m[13] - m[1] * m[8] * m[14] - m[2] * m[9] * m[12]));
            invm[index++] = (T) (inv_det * (m[0] * m[6] * m[13] + m[1] * m[4] * m[14] + m[2] * m[5] * m[12] - m[0] * m[5] * m[14] - m[1] * m[6] * m[12] - m[2] * m[4] * m[13]));
            invm[index++] = (T) (inv_det * (m[0] * m[5] * m[10] + m[1] * m[6] * m[8] + m[2] * m[4] * m[9] - m[0] * m[6] * m[9] - m[1] * m[4] * m[10] - m[2] * m[5] * m[8]));

        } else {

            invm[index++] = (T) (inv_det * (m[5] * m[10] * m[15] + m[6] * m[11] * m[13] + m[7] * m[9] * m[14] - m[5] * m[11] * m[14] - m[6] * m[9] * m[15] - m[7] * m[10] * m[13]));
            invm[index++] = (T) (inv_det * (m[1] * m[11] * m[14] + m[2] * m[9] * m[15] + m[3] * m[10] * m[13] - m[1] * m[10] * m[15] - m[2] * m[11] * m[13] - m[3] * m[9] * m[14]));
            invm[index++] = (T) (inv_det * (m[1] * m[6] * m[15] + m[2] * m[7] * m[13] + m[3] * m[5] * m[14] - m[1] * m[7] * m[14] - m[2] * m[5] * m[15] - m[3] * m[6] * m[13]));
//            invm[index++] = (T) (inv_det * (m[1] * m[7] * m[10] + m[2] * m[5] * m[11] + m[3] * m[6] * m[9] - m[1] * m[6] * m[11] - m[2] * m[7] * m[9] - m[3] * m[5] * m[10]));

            invm[index++] = (T) (inv_det * (m[4] * m[11] * m[14] + m[6] * m[8] * m[15] + m[7] * m[10] * m[12] - m[4] * m[10] * m[15] - m[6] * m[11] * m[12] - m[7] * m[8] * m[14]));
            invm[index++] = (T) (inv_det * (m[0] * m[10] * m[15] + m[2] * m[11] * m[12] + m[3] * m[8] * m[14] - m[0] * m[11] * m[14] - m[2] * m[8] * m[15] - m[3] * m[10] * m[12]));
            invm[index++] = (T) (inv_det * (m[0] * m[7] * m[14] + m[2] * m[4] * m[15] + m[3] * m[6] * m[12] - m[0] * m[6] * m[15] - m[2] * m[7] * m[12] - m[3] * m[4] * m[14]));
//            invm[index++] = (T) (inv_det * (m[0] * m[6] * m[11] + m[2] * m[7] * m[8] + m[3] * m[4] * m[10] - m[0] * m[7] * m[10] - m[2] * m[4] * m[11] - m[3] * m[6] * m[8]));

            invm[index++] = (T) (inv_det * (m[4] * m[9] * m[15] + m[5] * m[11] * m[12] + m[7] * m[8] * m[13] - m[4] * m[11] * m[13] - m[5] * m[8] * m[15] - m[7] * m[9] * m[12]));
            invm[index++] = (T) (inv_det * (m[0] * m[11] * m[13] + m[1] * m[8] * m[15] + m[3] * m[9] * m[12] - m[0] * m[9] * m[15] - m[1] * m[11] * m[12] - m[3] * m[8] * m[13]));
            invm[index++] = (T) (inv_det * (m[0] * m[5] * m[15] + m[1] * m[7] * m[12] + m[3] * m[4] * m[13] - m[0] * m[7] * m[13] - m[1] * m[4] * m[15] - m[3] * m[5] * m[12]));
//            invm[index++] = (T) (inv_det * (m[0] * m[7] * m[9] + m[1] * m[4] * m[11] + m[3] * m[5] * m[8] - m[0] * m[5] * m[11] - m[1] * m[7] * m[8] - m[3] * m[4] * m[9]));

            invm[index++] = (T) (inv_det * (m[4] * m[10] * m[13] + m[5] * m[8] * m[14] + m[6] * m[9] * m[12] - m[4] * m[9] * m[14] - m[5] * m[10] * m[12] - m[6] * m[8] * m[13]));
            invm[index++] = (T) (inv_det * (m[0] * m[9] * m[14] + m[1] * m[10] * m[12] + m[2] * m[8] * m[13] - m[0] * m[10] * m[13] - m[1] * m[8] * m[14] - m[2] * m[9] * m[12]));
            invm[index++] = (T) (inv_det * (m[0] * m[6] * m[13] + m[1] * m[4] * m[14] + m[2] * m[5] * m[12] - m[0] * m[5] * m[14] - m[1] * m[6] * m[12] - m[2] * m[4] * m[13]));
//            invm[index++] = (T) (inv_det * (m[0] * m[5] * m[10] + m[1] * m[6] * m[8] + m[2] * m[4] * m[9] - m[0] * m[6] * m[9] - m[1] * m[4] * m[10] - m[2] * m[5] * m[8]));

        }

    }

public:

    /**
     * 逆行列を計算し、resultに格納する
     */
    inline void invert(Matrix<ROW, COLM> *result) const {
        Matrix<4, 4> temp;
        for (int i = 0; i < ROW; ++i) {
            for (int k = 0; k < COLM; ++k) {
                temp.m[i][k] = m[i][k];
            }
        }

        _invert<float>((const float*) temp.m, (float*) result->m);
    }

    inline void invert() {
        invert(this);
    }

    /**
     * 配列への直接アクセス
     */
    inline float* operator[](const int row) const {
        return (float*) m[row];
    }

};

typedef Matrix<4, 3> Matrix4x3;
typedef Matrix<4, 4> Matrix4x4;

/**
 * 行列乗算を行う
 */
inline static Matrix<4, 4>* multiply(const Matrix<4, 4> &before, const Matrix<4, 4> &after, Matrix<4, 4> *result) {

    // テンポラリ領域
    float temp[4][4];

    after.multiply4(before.m[0], temp[0]);
    after.multiply4(before.m[1], temp[1]);
    after.multiply4(before.m[2], temp[2]);
    after.multiply4(before.m[3], temp[3]);

    memcpy(result->m, temp, sizeof(temp));
    return result;
}

/**
 * 行列乗算を行う
 */
inline static Matrix<4, 3>* multiply(const Matrix<4, 3> &before, const Matrix<4, 3> &after, Matrix<4, 3> *result) {

    // テンポラリ領域
    float temp[4][3];

    after.multiply3(before.m[0], 0, temp[0]);
    after.multiply3(before.m[1], 0, temp[1]);
    after.multiply3(before.m[2], 0, temp[2]);
    after.multiply3(before.m[3], 1, temp[3]);

    memcpy(result->m, temp, sizeof(temp));
    return result;
}

}
#endif /* JCMATRIX4X4_H_ */
