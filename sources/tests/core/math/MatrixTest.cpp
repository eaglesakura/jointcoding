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
