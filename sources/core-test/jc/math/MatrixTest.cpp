/*
 * MatrixTest.cpp
 *
 *  Created on: 2012/12/21
 */

#include    "jointcoding-testsupport.h"
#include    "jc/math/Matrix.h"
#include    "boost/numeric/ublas/matrix.hpp"

//    boost::numeric::ublas::detail::matrix<float> boost_mat(4, 4);

using namespace jc;

TESTFUNCTION void test_matrix_invert() {

    for (int i = 0; i < 100; ++i) {
        Matrix<4, 4> m;
        Matrix<4, 4> inv;
        Matrix<4, 4> blend;

        m.rotate(0, 1, 0, i * 3);
        m.translate(i * 1, i * 100, i * 10);

        m.invert(&inv);

        {

            // blend
            multiply(m, inv, &blend);
            if (!blend.isIdentity()) {
                jclogf("loop(%d)", i);
                blend.print();
            }

            _assertTrue(blend.isIdentity(0.001f));
        }
    }

    for (int i = 0; i < 100; ++i) {
        Matrix<4, 3> m;
        Matrix<4, 3> inv;
        Matrix<4, 3> blend;

        m.rotate(0, 1, 0, i * 3);
        m.translate(i * 1, i * 100, i * 10);

        m.invert(&inv);

        {

            // blend
            multiply(m, inv, &blend);
            if (!blend.isIdentity()) {
                jclogf("loop(%d)", i);
                blend.print();
            }

            _assertTrue(blend.isIdentity(0.001f));
        }
    }

}

void test_matrix_transform() {
    const float tX = 1;
    const float tY = 2;
    const float tZ = 3;

// check 4x4
    {
        Matrix<4, 4> m;
        Vector3f v;
        m.translate(tX, tY, tZ);
        m.multiply(v, &v);
        m.print();

        // matrixの移動が正しいことをチェック
        _assertEquals(m[3][0], tX);
        _assertEquals(m[3][1], tY);
        _assertEquals(m[3][2], tZ);

        // transformの結果が正しいことをチェック
        _assertEquals(v.x, tX);
        _assertEquals(v.y, tY);
        _assertEquals(v.z, tZ);
    }

// check 4x3
    {
        Matrix<4, 3> m;
        Vector3f v;
        m.translate(tX, tY, tZ);
        m.multiply(v, &v);
        m.print();

        // matrixの移動が正しいことをチェック
        _assertEquals(m[3][0], tX);
        _assertEquals(m[3][1], tY);
        _assertEquals(m[3][2], tZ);

        // transformの結果が正しいことをチェック
        _assertEquals(v.x, tX);
        _assertEquals(v.y, tY);
        _assertEquals(v.z, tZ);
    }
}

void test_matrix_rotate() {
    const float degree = 90;

// check 4x4
    {
        Matrix<4, 4> m;
        Vector3f v(0, 1, 0);
        m.rotate(1, 0, 0, degree);
        m.multiply(v, &v);
        m.print();

        // transformの結果が正しいことをチェック
        jclogf("rotate vec(%f, %f, %f)", v.x, v.y, v.z);

        // matrixの移動が正しいことをチェック
        // 上向きのベクトルをX回転90度させたら、Z方向に点は向いている
        _assertTrue(jc::equals(0.0f, v.x));
        _assertTrue(jc::equals(0.0f, v.y));
        _assertTrue(jc::equals(-1.0f, v.z));
    }
    jclog("-----");
// check 4x4
    {
        Matrix<4, 4> m;
        Vector3f v(1, 0, 0);
        m.rotate(0, 1, 0, degree);
        m.multiply(v, &v);
        m.print();

        // transformの結果が正しいことをチェック
        jclogf("rotate vec(%f, %f, %f)", v.x, v.y, v.z);

        // matrixの移動が正しいことをチェック
        // 横向きのベクトルをY方向に90度傾けたら、Z方向に点は向いている
        _assertTrue(jc::equals(0.0f, v.x));
        _assertTrue(jc::equals(0.0f, v.y));
        _assertTrue(jc::equals(1.0f, v.z));
    }
    jclog("-----");
// check 4x4
    {
        Matrix<4, 4> m;
        Vector3f v(0, 1, 0);
        m.rotate(0, 0, 1, degree);
        m.multiply(v, &v);
        m.print();

        // transformの結果が正しいことをチェック
        jclogf("rotate vec(%f, %f, %f)", v.x, v.y, v.z);

        // matrixの移動が正しいことをチェック
        // 上向きのベクトルをZ方向に90度傾けたら、X方向に点は向いている
        _assertTrue(jc::equals(-1.0f, v.x));
        _assertTrue(jc::equals(0.0f, v.y));
        _assertTrue(jc::equals(0.0f, v.z));
    }
}

void test_matrix_scaling() {
    const float scale_x = 1.0f;
    const float scale_y = 2.0f;
    const float scale_z = 0.5f;

// check 4x4
    {
        Matrix<4, 4> m;
        Vector3f v(1, 2, 3);
        m.scale(scale_x, scale_y, scale_z);
        m.multiply(v, &v);
        m.print();

        // matrixの移動が正しいことをチェック
        _assertEquals(m[0][0], scale_x);
        _assertEquals(m[1][1], scale_y);
        _assertEquals(m[2][2], scale_z);

        // transformの結果が正しいことをチェック
        _assertEquals(v.x, 1.0f * scale_x);
        _assertEquals(v.y, 2.0f * scale_y);
        _assertEquals(v.z, 3.0f * scale_z);
    }

// check 4x3
    {
        Matrix<4, 3> m;
        Vector3f v(1, 2, 3);
        m.scale(scale_x, scale_y, scale_z);
        m.multiply(v, &v);
        m.print();

        // matrixの移動が正しいことをチェック
        _assertEquals(m[0][0], scale_x);
        _assertEquals(m[1][1], scale_y);
        _assertEquals(m[2][2], scale_z);

        // transformの結果が正しいことをチェック
        _assertEquals(v.x, 1.0f * scale_x);
        _assertEquals(v.y, 2.0f * scale_y);
        _assertEquals(v.z, 3.0f * scale_z);

    }
}

void test_matrix_multiply() {

// check 4x4
    {
        Matrix4x4 before;
        Matrix4x4 after;

        before.translate(1, 2, 3);
        after.scale(0.5f, 2.0f, 1.0f);

        Matrix4x4 blend;

        multiply(before, after, &blend);

        Vector3f vec(0, 0, 0);

        blend.multiply(vec, &vec);

        blend.print();
        jclogf("mult vec(%f, %f, %f)", vec.x, vec.y, vec.z);
        _assertTrue(equals(0.5f, vec.x));
        _assertTrue(equals(4.0f, vec.y));
        _assertTrue(equals(3.0f, vec.z));
    }

// check 4x4
    {
        Matrix4x4 before;
        Matrix4x4 after;

        before.scale(0.5f, 2.0f, 1.0f);
        after.translate(1, 2, 3);

        Matrix4x4 blend;

        multiply(before, after, &blend);

        Vector3f vec(1, 1, 1);

        blend.multiply(vec, &vec);

        blend.print();
        jclogf("mult vec(%f, %f, %f)", vec.x, vec.y, vec.z);
        _assertTrue(equals(1.0f + 0.5f, vec.x));
        _assertTrue(equals(2.0f + 2.0f, vec.y));
        _assertTrue(equals(3.0f + 1.0f, vec.z));
    }

// check 4x3
    {
        Matrix4x3 before;
        Matrix4x3 after;

        before.translate(1, 2, 3);
        after.scale(0.5f, 2.0f, 1.0f);

        Matrix4x3 blend;

        multiply(before, after, &blend);

        Vector3f vec(0, 0, 0);

        blend.multiply(vec, &vec);

        blend.print();
        jclogf("mult vec(%f, %f, %f)", vec.x, vec.y, vec.z);
        _assertTrue(equals(0.5f, vec.x));
        _assertTrue(equals(4.0f, vec.y));
        _assertTrue(equals(3.0f, vec.z));
    }

// check 4x3
    {
        Matrix4x3 before;
        Matrix4x3 after;

        before.scale(0.5f, 2.0f, 1.0f);
        after.translate(1, 2, 3);

        Matrix4x3 blend;

        multiply(before, after, &blend);

        Vector3f vec(1, 1, 1);

        blend.multiply(vec, &vec);

        blend.print();
        jclogf("mult vec(%f, %f, %f)", vec.x, vec.y, vec.z);
        _assertTrue(equals(1.0f + 0.5f, vec.x));
        _assertTrue(equals(2.0f + 2.0f, vec.y));
        _assertTrue(equals(3.0f + 1.0f, vec.z));
    }
}
