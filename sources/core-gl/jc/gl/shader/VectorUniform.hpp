/*
 * VectorUniform.hpp
 *
 *  Created on: 2013/04/20
 */

#ifndef VECTORUNIFORM_HPP_
#define VECTORUNIFORM_HPP_

#include    "jc/math/Vec2.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Vec4.h"
#include    "jc/gl/shader/UniformBase.hpp"

namespace jc {
namespace gl {

/**
 * ベクトル型のUniformをサポートする
 * 無効なUniform値（コンパイラ最適化で削除された等）の場合はアップロードを無視する
 *
 * 転送されるとき、シェーダーのユニットはvec4単位で確保されるため、効率的に利用する場合はシェーダー側で適当に配置を考えてやる必要がある。
 * 上手いことスキマに配置してくれるハズだが、ユニット数の残量には気をつけること。
 */
template<typename vector_type, u32 vector_length>
class VectorUniform: public UniformBase {
    /**
     * 保持するベクトルユニット
     */
    vector_type vec[vector_length];
public:
    VectorUniform() {
        // 0で初期化する
        for (int i = 0; i < vector_length; ++i) {
            vec[i] = (vector_type) 0;
        }
    }

    VectorUniform(MGLShaderProgram program, const charactor *name) {
        // 0で初期化する
        for (int i = 0; i < vector_length; ++i) {
            vec[i] = (vector_type) 0;
        }

        setLocation(program, name);
    }

    ~VectorUniform() {
    }

    /**
     * 要素を取得する
     */
    vector_type get(const u32 index) const {
        assert(vector_length > index);

        return vec[index];
    }

    /**
     * float値を1つ転送する
     */
    jcboolean upload(const float x) {
        assert(vector_length >= 1);

        if (!valid()) {
            return jcfalse;
        }

        if (vec[0] != x) {
            glUniform1f(location, x);
            assert_gl();
            vec[0] = x;
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * float値を2つ転送する
     */
    jcboolean upload(const float x, const float y) {
        assert(vector_length >= 2);

        if (!valid()) {
            return jcfalse;
        }

        if (vec[0] != x || vec[1] != y) {
            glUniform2f(location, x, y);
            assert_gl();
            vec[0] = x;
            vec[1] = y;
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * ベクトルを転送する
     */
    jcboolean upload(const Vector2f &v) {
        return upload(v.x, v.y);
    }

    /**
     * float値を3つ転送する
     */
    jcboolean upload(const float x, const float y, const float z) {
        assert(vector_length >= 3);

        if (!valid()) {
            return jcfalse;
        }

        if (vec[0] != x || vec[1] != y || vec[3] != z) {
            glUniform3f(location, x, y, z);
            assert_gl();
            vec[0] = x;
            vec[1] = y;
            vec[2] = z;
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * ベクトルを転送する
     */
    jcboolean upload(const Vector3f &v) {
        return upload(v.x, v.y, v.z);
    }

    /**
     * float値を4つ転送する
     */
    jcboolean upload(const float x, const float y, const float z, const float w) {
        assert(vector_length >= 4);

        if (!valid()) {
            return jcfalse;
        }

        if (vec[0] != x || vec[1] != y || vec[2] != z || vec[3] != w) {
            glUniform4f(location, x, y, z, w);
            assert_gl();
            vec[0] = x;
            vec[1] = y;
            vec[2] = z;
            vec[3] = w;
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * float値を4つ転送する
     */
    jcboolean upload(const Vector4f &v) {
        return upload(v.x, v.y, v.z, v.w);
    }

    /**
     * 44行列を直接転送する
     */
    jcboolean upload(const float *pMatrix) {
        assert(vector_length >= 16);
        assert(pMatrix != NULL);

        if (!valid()) {
            return jcfalse;
        }

        jcboolean upload = jcfalse;
        // 転送チェック
        for (int i = 0; i < vector_length; ++i) {
            if (vec[i] != pMatrix[i]) {
                vec[i] = pMatrix[i];
                upload = jctrue;
            }
        }

        if (!upload) {
            // アップロードの必要がない
            return jcfalse;
        }

        // 行列の数だけ転送する
        glUniformMatrix4fv(location, vector_length / 16, GL_FALSE, pMatrix);
        assert_gl();

        return jctrue;
    }

    /**
     * 圧縮状態で行列を転送することを許可する
     * 4x4行列のうち、3x4部分のみを使用している場合、4x3に転地してvec4[3]状態にすることでベクトルユニットを節約できる
     * 大量に行列を扱う必要が有る場合に有用だが、頂点シェーダーの負荷が増すため利用シーンには注意すること
     *
     * FIXME 未実装で、将来的な変更に対応しておく
     */
    jcboolean uploadCompless(const float *pMatrix) {
        return upload(pMatrix);
    }

    /**
     * 44行列を転送する
     */
    jcboolean upload(const Matrix4x4 &m) {
        return upload((const float*) (&m));
    }
};

/**
 * uniform float
 */
typedef VectorUniform<float, 1> FloatUniform;

/**
 * uniform float[2]
 * uniform vec2
 */
typedef VectorUniform<float, 2> Vector2fUniform;

/**
 * uniform float[3]
 * uniform vec3
 */
typedef VectorUniform<float, 3> Vector3fUniform;

/**
 * uniform float[4]
 * uniform vec4
 */
typedef VectorUniform<float, 4> Vector4fUniofrm;

/**
 * uniform mat4
 */
typedef VectorUniform<float, 16> MatrixUniform;
}
}

#endif /* VECTORUNIFORM_HPP_ */
