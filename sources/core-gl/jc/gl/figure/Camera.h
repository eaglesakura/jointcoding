/*
 * Camera.h
 *
 *  Created on: 2013/01/18
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include    "jointcoding.h"
#include    "jc/math/Vec3.h"
#include    "jc/math/Matrix.h"

namespace jc {
namespace gl {

class Camera: public Object {
    /**
     * 現在位置
     */
    Vector3f position;

    /**
     * 注視点
     */
    Vector3f look;

    /**
     * アップベクトル
     */
    Vector3f up;

    /**
     * Y方向画角
     */
    float fovY;

    /**
     * ニアクリップ
     */
    float near;

    /**
     * ファークリップ
     */
    float far;

    /**
     * カメラアスペクト
     */
    float aspect;
public:
    Camera() {
        position.set(0, 1, 1);
        look.set(0, 0, 0);
        up.set(0, 1, 0);
        fovY = 45.0f;
        near = 1.0f;
        far = 1000.0f;
        aspect = 480 / 800;
    }

    virtual ~Camera() {
    }

    /**
     * カメラ用行列を取得する。
     */
    Matrix4x4* getMatrix(Matrix4x4 *result) const {
        result->identity();
        result->lookAt(position, look, up);

        {
            Matrix4x4 prj;
            prj.projection(near, far, fovY, aspect);
            multiply(*result, prj, result);
        }

        return result;
    }

    virtual void setPosition(const float x, const float y, const float z) {
        position.set(x, y, z);
    }

    virtual void setLook(const float x, const float y, const float z) {
        look.set(x, y, z);
    }

    virtual void setFovY(const float set) {
        this->fovY = set;
    }

    virtual void setProjection(const float near, const float far, const float aspect) {
        this->near = near;
        this->far = far;
        this->aspect = aspect;
    }
};

typedef jc_sp<Camera> MCamera;

}
}

#endif /* CAMERA_H_ */
