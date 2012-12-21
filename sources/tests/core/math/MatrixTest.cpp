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
