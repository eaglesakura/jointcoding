/*
 * Camera.h
 *
 *  Created on: 2013/01/18
 */

#ifndef CAMERAASSET_H_
#define CAMERAASSET_H_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * 行列の処理キャッシュを管理する構造体
 */
struct MatrixCache {
    /**
     * 行列本体
     */
    Matrix4x4 m;

    /**
     * この行列が古い場合はtrue
     */
    jcboolean old;

    MatrixCache() {
        old = jctrue;
    }
};

/**
 * カメラオブジェクト
 * 内部ではカメラ行列のキャッシュを持ち、複数回のブレンドを行わないように適当に制御する
 */
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

    /**
     * 視線行列
     */
    mutable MatrixCache lookMatrix;

    /**
     * 射影行列
     */
    mutable MatrixCache prjMatrix;

    /**
     * ブレンド済みカメラ行列
     */
    mutable Matrix4x4 lookprojection;
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
     * カメラ行列を取得する
     */
    const Matrix4x4& getLookProjectionMatrix() const {
        // 行列の更新フラグ
        // trueになった場合はlookprjも更新する
        jcboolean materix_update = jcfalse;
        // 各行列を更新する
        if (lookMatrix.old) {
            lookMatrix.m.lookAt(position, look, up);
            lookMatrix.old = jcfalse;
            materix_update = jctrue;
        }
        if (prjMatrix.old) {
            prjMatrix.m.projection(near, far, fovY, aspect);
            prjMatrix.old = jcfalse;
            materix_update = jctrue;
        }

        if (materix_update) {
            multiply(lookMatrix.m, prjMatrix.m, &lookprojection);
        }
        return lookprojection;
    }

    /**
     * 上方ベクトルを設定する
     */
    virtual void setUp(const float x, const float y, const float z) {
        up.set(x, y, z);
        up.normalize();
        lookMatrix.old = jctrue;
    }

    /**
     * 上方ベクトルを設定する
     */
    virtual void setUp(const Vector3f &v) {
        setUp(v.x, v.y, v.z);
    }

    /**
     * カメラ位置を設定する
     */
    virtual void setPosition(const float x, const float y, const float z) {
        position.set(x, y, z);
        lookMatrix.old = jctrue;
    }

    /**
     * カメラ位置を設定する
     */
    virtual void setPosition(const Vector3f &v) {
        setPosition(v.x, v.y, v.z);
    }

    /**
     * カメラ注視点を設定する
     */
    virtual void setLook(const float x, const float y, const float z) {
        look.set(x, y, z);
        lookMatrix.old = jctrue;
    }

    /**
     * カメラ注視点を設定する
     */
    virtual void setLook(const Vector3f &v) {
        setLook(v.x, v.y, v.z);
    }

    /**
     * 視線情報を一度に登録する
     */
    virtual void lookAt(const Vector3f &pos, const Vector3f &look, const Vector3f &up) {
        setPosition(pos);
        setLook(look);
        setUp(up);
    }

    /**
     * 画角を指定する
     */
    virtual void setFovY(const float set) {
        this->fovY = set;
        lookMatrix.old = jctrue;
    }

    /**
     * 射影情報を指定する
     */
    virtual void setProjection(const float near, const float far, const float aspect) {
        this->near = near;
        this->far = far;
        this->aspect = aspect;
        lookMatrix.old = jctrue;
    }

    /**
     * 射影情報を一度に登録する
     */
    virtual void projection(const float near, const float far, const float fovY, const float aspect) {
        this->fovY = fovY;
        setProjection(near, far, aspect);
    }
};

typedef jc_sp<Camera> MCamera;

}
}

#endif /* CAMERAASSET_H_ */
