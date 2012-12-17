/*
 * EulerTransform.h
 *
 *  Created on: 2012/12/14
 */

#ifndef EULERTRANSFORM_H_
#define EULERTRANSFORM_H_

#include    "jc/math/Vec3.h"

namespace jc {

enum EulerRotateOrder_e {
    /**
     * X->Y->Z順に適用
     */
    EulerRotateOrder_XYZ = 0,
    /**
     * X->Z->Y順に適用
     */
    EulerRotateOrder_XZY,
    /**
     * Y->X->Z順に適用
     */
    EulerRotateOrder_YXZ,
    /**
     * Y->Z->X順に適用
     */
    EulerRotateOrder_YZX,
    /**
     * Z->X->Y順に適用
     */
    EulerRotateOrder_ZXY,
    /**
     * Z->Y->X順に適用
     */
    EulerRotateOrder_ZYX,
};

class EulerTransform {
public:
    /**
     * ノードの位置
     */
    Vector3f translate;

    /**
     * ノードの回転角
     */
    Vector3f rotate;

    /**
     * 回転の適用順番を定義する
     */
    EulerRotateOrder_e rotateType;

    /**
     * ノードのスケーリング値
     */
    Vector3f scale;

    EulerTransform() {
        rotateType = EulerRotateOrder_XYZ;
    }

    ~EulerTransform() {

    }
};

}

#endif /* EULERTRANSFORM_H_ */
