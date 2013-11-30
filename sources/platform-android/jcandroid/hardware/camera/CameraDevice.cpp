/*
 * CameraDevice.cpp
 *
 *  Created on: 2013/06/27
 */

#include "CameraDevice.h"

#include "/dev-home/android/ndk/platforms/android-14/arch-arm/usr/include/assert.h"
#include "/dev-home/android/ndk/platforms/android-14/arch-arm/usr/include/GLES2/gl2ext.h"
#include "/dev-home/android/ndk/platforms/android-14/arch-arm/usr/include/jni.h"
#include "/dev-home/android/ndk/platforms/android-14/arch-arm/usr/include/string.h"
#include "../../../../core/boost/smart_ptr/shared_ptr.hpp"
#include "../../../../core/jc/math/Matrix.h"
#include "../../../../core/jc/mem/SmartPtr.h"
#include "../../../../core/jc/system/Log.h"
#include "../../../../core/jc/system/Macro.h"
#include "../../../../core-gl/jc/gl/context/State.h"
#include "../../../../core-gl/jc/gl/gpu/Device.h"
#include "../../../../core-gl/jc/gl/texture/TextureImage.h"
#include "../../../../gen/android-classes/ndkCameraWrapper.h"
#include "../../ndk-support.h"

namespace ndk {

CameraDevice::CameraDevice(const s32 CameraDeviceProtocol_TYPE) {
    this->CameraDeviceProtocol_TYPE = CameraDeviceProtocol_TYPE;
    this->previewStarted = jcfalse;
}

CameraDevice::~CameraDevice() {
    this->dispose();
}

/**
 * プレビューの幅を取得する
 */
s32 CameraDevice::getPreviewWidth() const {
    if (!previewTexture) {
        return 0;
    }

    return nativeCamera->getPreviewWidth();
}

/**
 * プレビューの高さを取得する
 */
s32 CameraDevice::getPreviewHeight() const {
    if (!previewTexture) {
        return 0;
    }

    return nativeCamera->getPreviewHeight();
}

/**
 * テクスチャに対してプレビューを開始する
 */
MTextureImage CameraDevice::startPreview(MDevice device) {
    // 既にプレビュー済みではいけない
    assert(!previewTexture);

    previewTexture.reset(new TextureImage(GL_TEXTURE_EXTERNAL_OES, 1, 1, device));

    // プレビュー開始を行わせる
    if (nativeCamera->startPreview(previewTexture->getName())) {
        jclog("preview start complete");
    } else {
        jclog("preview start failed");
        // プレビューに失敗した
        previewTexture.reset();
    }

    return previewTexture;
}

/**
 * プレビュー状態を更新する
 */
void CameraDevice::updatePreview(MGLState state) {
    if (!previewTexture) {
        return;
    }

    // テクスチャを更新する
    // 一度でも成功していたら、プレビュー開始を許可する
    previewTexture->bind(state);
    if (nativeCamera->renderingToTexture()) {
        previewStarted = jctrue;
    }
}

/**
 * プレビューを終了する
 */
void CameraDevice::stopPreview() {
    if (!previewTexture) {
        return;
    }

    previewStarted = jcfalse;
    nativeCamera->stopPreview();
    previewTexture.reset();
}

/**
 * プレビューサイズを要求するする
 */
void CameraDevice::requestPreviewSize(const s32 width, const s32 height, const s32 minWidth, const s32 minHeight) {
    nativeCamera->requestPreviewSize(width, height, minWidth, minHeight, (height > width));
}

/**
 * 回転角を設定する
 */
void CameraDevice::requestRotation(const s32 CameraDeviceProtocol_ORIENTATION) {
    nativeCamera->requestOrientation(CameraDeviceProtocol_ORIENTATION);
}

/**
 * カメラを明示的に解放する
 */
void CameraDevice::dispose() {
    if (!nativeCamera) {
        return;
    }

    stopPreview();
    nativeCamera->dispose();
    nativeCamera.reset();
}

/**
 * プレビューテクスチャ用のUV行列を取得する
 */
Matrix4x4* CameraDevice::getPreviewMatrix(Matrix4x4 *pResult) {
    assert(pResult);

    jobject jFloatBuffer = nativeCamera->getTextureMatrix_unsafe();
    if (jFloatBuffer) {
        CALL_JNIENV();

        const void* matrixBuffer = env->GetDirectBufferAddress(jFloatBuffer);
        assert(matrixBuffer);

        // バッファをNative側へコピーする
        memcpy((void*) pResult->m, matrixBuffer, sizeof(Matrix4x4));

        // 削除
        env->DeleteLocalRef(jFloatBuffer);
    }
    return pResult;
}

/**
 * カメラデバイスを取得する
 */
jc_sp<CameraDevice> CameraDevice::createInstance(const s32 CameraDeviceProtocol_TYPE) {

    jc_sp<CameraWrapper> nativeCamera = CameraWrapper::global(CameraWrapper::createInstance_unsafe(CameraDeviceProtocol_TYPE));
    if(!nativeCamera) {
        return jc_sp<CameraDevice>();
    }

    jc_sp<CameraDevice> result(new CameraDevice(CameraDeviceProtocol_TYPE));
    result->nativeCamera = nativeCamera;

    return result;
}

}

