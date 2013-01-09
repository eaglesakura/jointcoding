/*
 * EulerTransform.h
 *
 *  Created on: 2012/12/14
 */

#ifndef EULERTRANSFORM_H_
#define EULERTRANSFORM_H_

#include    "jc/math/Vec3.h"
#include    "jc/math/Vec4.h"
#include    "jc/math/Matrix.h"

namespace jc {

enum TransformRotateOrder_e {
    /**
     * X->Y->Z順に適用
     */
    TransformRotateOrder_XYZ = 0,
    /**
     * X->Z->Y順に適用
     */
    TransformRotateOrder_XZY,
    /**
     * Y->X->Z順に適用
     */
    TransformRotateOrder_YXZ,
    /**
     * Y->Z->X順に適用
     */
    TransformRotateOrder_YZX,
    /**
     * Z->X->Y順に適用
     */
    TransformRotateOrder_ZXY,
    /**
     * Z->Y->X順に適用
     */
    TransformRotateOrder_ZYX,

    /**
     * Ex.クォータニオンとして作成する
     */
    TransformRotateOrder_Quaternion = -1,

};

class Transform: public Object {
public:
    /**
     * ノードの位置
     */
    Vector3f translate;

    /**
     * ノードの回転角
     */
    Vector4f rotate;

    /**
     * 回転の適用順番を定義する
     */
    TransformRotateOrder_e rotateType;

    /**
     * ノードのスケーリング値
     */
    Vector3f scale;

    Transform() {
        scale.set(1, 1, 1);
        rotateType = TransformRotateOrder_XYZ;
    }

    virtual ~Transform() {
    }


    /**
     * thisを構築する行列を作成する
     */
    Matrix4x4* getMatrix(Matrix4x4 *result) const {
        Matrix4x4 temp;
        result->identity();

        if (scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f) {
            result->scale(scale.x, scale.y, scale.z);
        }

        //! x
        if (rotate.x != 0.0f) {
            temp.rotateX(rotate.x);
//            temp.rotate(-1.0f, 0.0f, 0.0f, rotate.x);
            multiply(*result, temp, result);
        }

        //! y
        if (rotate.y != 0.0f) {
            temp.identity();
//            temp.rotate(0.0f, -1.0f, 0.0f, rotate.y);
            temp.rotateY(rotate.y);
            multiply(*result, temp, result);
        }

        //! z
        if (rotate.z != 0.0f) {
            temp.identity();
//            temp.rotate(0.0f, 0.0f, -1.0f, rotate.z);
            temp.rotateZ(rotate.z);
            multiply(*result, temp, result);
        }

        // position
        {
            temp.identity();
            temp.translate(translate.x, translate.y, translate.z);
            multiply(*result, temp, result);
        }


        return result;
    }
    /**
     * thisを構築する行列の逆行列を取得する
     */
    Matrix4x4* getMatrixInvert(Matrix4x4 *result) const {
        getMatrix(result);
        result->invert();
        return result;
    }

};

}

#endif /* EULERTRANSFORM_H_ */
