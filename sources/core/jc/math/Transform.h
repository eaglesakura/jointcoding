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

/**
 * 位置を決めるための行列生成器
 */
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
        rotateType = TransformRotateOrder_XZY;
    }

    virtual ~Transform() {
    }

    /**
     * thisを構築する行列を作成する
     */
    Matrix4x4* getMatrix(Matrix4x4 *result) const {
        // 現状でサポートしているのは3ds MaxのXZY形式のみ
        assert(rotateType == TransformRotateOrder_XZY);

        Matrix4x4 scale_m;
        Matrix4x4 rotateX;
        Matrix4x4 rotateY;
        Matrix4x4 rotateZ;
        Matrix4x4 translate_m;
        result->identity();

        if (scale.x != 1.0f || scale.y != 1.0f || scale.z != 1.0f) {
            scale_m.scale(scale.x, scale.y, scale.z);
        }

        //! x
        if (rotate.x != 0.0f) {
            rotateX.rotateX(rotate.x);
        }

        //! z
        if (rotate.z != 0.0f) {
            rotateZ.rotateZ(rotate.z);
        }

        //! y
        if (rotate.y != 0.0f) {
            rotateY.rotateY(rotate.y);
        }

        // position
        {
            translate_m.translate(translate.x, translate.y, translate.z);
        }

        multiply(scale_m, rotateX, result);
        multiply(*result, rotateZ, result);
        multiply(*result, rotateY, result);
        multiply(*result, translate_m, result);

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
