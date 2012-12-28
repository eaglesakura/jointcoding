/*
 * EulerTransform.h
 *
 *  Created on: 2012/12/14
 */

#ifndef EULERTRANSFORM_H_
#define EULERTRANSFORM_H_

#include    "jc/math/Vec3.h"
#include    "jc/math/Vec4.h"

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

class Transform : public Object {
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
        rotateType = TransformRotateOrder_XYZ;
    }

    virtual ~Transform() {
    }
};

}

#endif /* EULERTRANSFORM_H_ */
