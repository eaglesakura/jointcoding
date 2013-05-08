/*
 * EGLWrapper.cpp
 *
 *  Created on: 2013/05/08
 */

#include    "jcandroid/egl/sdk/EGLWrapper.h"
#include    "jcandroid/egl/sdk/EGLContextWrapper.hpp"
#include    "jcandroid/egl/sdk/EGLSurfaceWrapper.hpp"

#include    "jcandroid/egl/EGLError.hpp"

using namespace jc;
using namespace jc::gl;

namespace ndk {

SdkEGLWrapper::SdkEGLWrapper(const jc_sp<EGLWrapper> eglWrapper) {
    this->eglWrapper = eglWrapper;
}

SdkEGLWrapper::~SdkEGLWrapper() {

}

/**
 * ステータスを取得する
 */
EGLStatus_e SdkEGLWrapper::getStatus() const {
    MThreadID gl_thread = threadId;
    // スレッドIDを得ていなければ、どのスレッドにも属していない
    if (!gl_thread) {
        return EGLStatus_Ready;
    }

    // 現在のスレッドIDを取得
    ThreadID current_id;

    // 同一スレッドだったらAttached、それ以外のスレッドだったらBusy
    return current_id.equals(gl_thread) ? EGLStatus_Attached : EGLStatus_Busy;
}

/**
 * 指定したコンテキストとサーフェイスをカレントスレッドに割り当てる。
 */
void SdkEGLWrapper::current(jc_sp<EGLContextProtocol> context, jc_sp<EGLSurfaceProtocol> surface) {
    assert(eglWrapper);

    jcboolean completed = jcfalse;
    jobject jEGLSurface = NULL;;
    jobject jEGLContext = NULL;
    if(context && surface) {
        // コンテキストとサーフェイスが明示されている
        jc_sp<SdkEGLContextWrapper> contextWrapper = downcast<SdkEGLContextWrapper>(context);
        jc_sp<SdkEGLSurfaceWrapper> surfaceWrapper = downcast<SdkEGLSurfaceWrapper>(surface);
        assert(surfaceWrapper);
        assert(contextWrapper);

        jEGLContext = contextWrapper->eglContext->getObject();
        jEGLSurface = surfaceWrapper->eglSurface->getObject();

        assert(jEGLContext);
        assert(jEGLSurface);
    }

    completed = eglWrapper->current(jEGLContext, jEGLSurface);

    if(!completed) {
        // FIXME eglMakeCurrentに失敗した
    } else {
        if(jEGLContext) {
            // 現在のスレッドIDを指定する
            threadId.reset(new ThreadID());
        }else {
            // カレント情報を解除する
            threadId.reset();
        }
    }
}

/**
 * 描画用のディスプレイにバックバッファの内容を転送する。
 * 転送方法については各デバイスの実装に従う。
 * このメソッドは完了するまでブロックされることを保証しない（デバイス依存、基本的にブロッキングになるように調整している）
 */
jcboolean SdkEGLWrapper::postFrontBuffer(jc_sp<EGLSurfaceProtocol> displaySurface) {
    assert(eglWrapper);

    jc_sp<SdkEGLSurfaceWrapper> surfaceWrapper = downcast<SdkEGLSurfaceWrapper>(displaySurface);
    assert(surfaceWrapper);

    return eglWrapper->postFrontBuffer(surfaceWrapper->eglSurface->getObject());
}

/**
 * 握っているEGL資源を明示的に開放する。
 */
void SdkEGLWrapper::dispose() {
    eglWrapper.reset();
}

}
