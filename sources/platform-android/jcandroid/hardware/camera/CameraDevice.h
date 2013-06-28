/*
 * CameraDevice.h
 *
 *  Created on: 2013/06/27
 */

#ifndef CAMERADEVICE_H_
#define CAMERADEVICE_H_

#include    "jointcoding.h"
#include    "jointcoding-android.h"
#include    "protocol/jcCameraDeviceProtocol.h"

#include    "jc/gl/GL2D.h"

namespace ndk {

using namespace gl;

class CameraWrapper;

/**
 * カメラ用のハードウェアを抽象化する
 *
 * TODO 今のところAndroid向けに最適化しておき、後々iOSにも使えるようにカスタマイズしていく
 */
class CameraDevice: public Object {
    /**
     * カメラの種類
     */
    s32 CameraDeviceProtocol_TYPE;

    /**
     * プレビューの格納先
     */
    MTextureImage previewTexture;

    /**
     * プレビューが開始されていたらtrue
     */
    jcboolean previewStarted;

protected:
    /**
     * Native側カメラ
     */
    jc_sp<CameraWrapper> nativeCamera;

    CameraDevice(const s32 CameraDeviceProtocol_TYPE);
public:
    virtual ~CameraDevice();

    /**
     * カメラのデバイスタイプを取得する
     */
    virtual s32 getDeviceType() const {
        return CameraDeviceProtocol_TYPE;
    }

    /**
     * テクスチャに対してプレビューを開始する
     */
    virtual MTextureImage startPreview(MDevice device);

    /**
     * プレビューの幅を取得する
     */
    virtual s32 getPreviewWidth() const;

    /**
     * プレビューの高さを取得する
     */
    virtual s32 getPreviewHeight() const;

    /**
     * プレビュー状態を更新する
     */
    virtual void updatePreview();

    /**
     * プレビューを終了する
     */
    virtual void stopPreview();

    /**
     * プレビューテクスチャ用のUV行列を取得する
     */
    virtual Matrix4x4* getPreviewMatrix(Matrix4x4 *pResult);

    /**
     * プレビュー対象のテクスチャを取得する
     */
    virtual MTextureImage getPreveiwTexture() const {
        return previewTexture;
    }

    /**
     * プレビュー開始済みであればtrue
     */
    virtual jcboolean isPreviewStarted() const {
        return previewStarted && previewTexture;
    }

    /**
     * プレビューサイズを要求するする
     */
    virtual void requestPreviewSize(const s32 width, const s32 height, const s32 minWidth, const s32 minHeight);

    /**
     * カメラを明示的に解放する
     */
    virtual void dispose();
public:
    /**
     * カメラデバイスを取得する
     */
    static jc_sp<CameraDevice> createInstance(const s32 CameraDeviceProtocol_TYPE);
};

/**
 * managed
 */
typedef jc_sp<CameraDevice> MCameraDevice;

}

#endif /* CAMERADEVICE_H_ */
