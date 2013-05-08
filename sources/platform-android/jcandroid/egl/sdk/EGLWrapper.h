/*
 * sdkDeviceManager.hpp
 *
 *  Created on: 2013/05/07
 */

#ifndef SDKDEVICEMANAGER_HPP_
#define SDKDEVICEMANAGER_HPP_

#include    "jointcoding-android.h"
#include    "jc/egl/EGL.h"
#include    "jc/egl/EGLContext.h"
#include    "jc/egl/EGLSurface.h"

// SDK impl
#include    "android-classes/ndkEGLWrapper.h"

namespace ndk {
using namespace jc;
using namespace jc::gl;

class SdkEGLWrapper: public Object, public EGLProtocol {
    jc_sp<EGLWrapper> eglWrapper;

    /**
     * アタッチされているスレッドID
     */
    MThreadID threadId;

public:
    SdkEGLWrapper(const jc_sp<EGLWrapper> eglWrapper);

    virtual ~SdkEGLWrapper();

    /**
     * ステータスを取得する
     */
    virtual EGLStatus_e getStatus() const;

    /**
     * 指定したコンテキストとサーフェイスをカレントスレッドに割り当てる。
     */
    virtual void current(jc_sp<EGLContextProtocol> context, jc_sp<EGLSurfaceProtocol> surface);

    /**
     * 描画用のディスプレイにバックバッファの内容を転送する。
     * 転送方法については各デバイスの実装に従う。
     * このメソッドは完了するまでブロックされることを保証しない（デバイス依存、基本的にブロッキングになるように調整している）
     */
    virtual jcboolean postFrontBuffer(jc_sp<EGLSurfaceProtocol> displaySurface);

    /**
     * 握っているEGL資源を明示的に開放する。
     */
    virtual void dispose();
};

}

#endif /* SDKDEVICEMANAGER_HPP_ */
