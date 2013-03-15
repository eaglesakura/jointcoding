/*
 * jcglDevice.cpp
 *
 *  Created on: 2012/09/11
 */

#include    "jc/gl/Device.h"

namespace jc {
namespace gl {

Device::Device() {
    flags = 0;
    current_request = 0;
    current_id = 0;
}

Device::~Device() {
    jclogf("Device dispose %x", this);
}

/**
 * EGLの関連付けを設定し直す。
 */
jcboolean Device::makeCurrent(EGLMakeCurrent_e type) {
    switch (type) {
        case EGLMakeCurrent_Bind:
            // 廃棄フラグが設定されているなら、カレントに設定できない
            if (hasFlags(DeviceFlag_RequestDestroy)) {
                jclog("has destroy flag");
                return jcfalse;
            }

            // 既に廃棄済みでもカレントにできない
            if (context.get() == NULL || surface.get() == NULL) {
                jclogf("error context(%x) || surface(%x)", context.get(), surface.get());
                return jcfalse;
            }
            egl->current(context, surface);

            // 現在のスレッドIDに割り当てる
            threadId.reset(new ThreadID());
            return jctrue;
        case EGLMakeCurrent_Unbind:
            egl->current(EGL_NULL_CONTEXT, EGL_NULL_SURFACE);

            // スレッドIDを廃棄する
            threadId.reset();
            return jctrue;
    }
    return jcfalse;
}

/**
 * フロントバッファへサーフェイスを転送する。
 * DeviceLockを得ている必要がある。
 */
jcboolean Device::postFrontBuffer() {
    if (hasFlags(DeviceFlag_RequestDestroy)) {
        return jcfalse;
    }

    return egl->postFrontBuffer(surface);
}

/**
 * 管理しているリソースを解放する。
 */
void Device::dispose() {
    // 廃棄フラグを立てる
    flags |= DeviceFlag_RequestDestroy;

    // レンダリング用サーフェイスも強制廃棄
    if (surface) {
        surface->dispose();
        surface.reset();
    }

    // レンダリング用コンテキストは強制廃棄
    if (context) {
        context->dispose();
        context.reset();
    }

    if (egl) {
        egl.reset();
    }

    // スレッドの廃棄を行う
    threadId.reset();
}

}
}
