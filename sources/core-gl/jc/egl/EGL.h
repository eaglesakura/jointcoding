/*
 * jcEGL.h
 *
 *  Created on: 2012/06/30
 *      Author: Takeshi
 */

#ifndef JCEGL_H_
#define JCEGL_H_

#include    "jc/system/Macro.h"
#include    "jc/mem/SmartPtr.h"
#include    "jc/thread/ThreadId.h"

namespace jc {

namespace gl {

/**
 * GLのサーフェイス保持クラスのプロトコル
 */
class EGLSurfaceProtocol;

/**
 * GLのステートを保持クラスのプロトコル
 */
class EGLContextProtocol;

/**
 * EGL初期化状態を取得する
 */
enum EGLStatus_e {
    /**
     * 初期化が終わっていない状態
     */
    EGLStatus_NotInitialized,

    /**
     * どのスレッドにもコンテキストがアタッチされていない状態
     */
    EGLStatus_Ready,

    /**
     * 現在のスレッドにアタッチされている
     */
    EGLStatus_Attached,

    /**
     * 別なスレッドにアタッチされている
     */
    EGLStatus_Busy,
};

/**
 * GLを初期化した対象のネイティブViewの情報を取得する。
 * EGL相当の動作を行う
 * EGL資源は常に1スレッドに対しして占有権があり、それ以外のスレッドでは利用できない。
 * 内部的にはgpu_mutexを利用して排他制御・資源割り当てを行う。
 */
class EGLProtocol {
public:
    virtual ~EGLProtocol() {
    }

    /**
     * ステータスを取得する
     */
    virtual EGLStatus_e getStatus() const = 0;

    /**
     * 指定したコンテキストとサーフェイスをカレントスレッドに割り当てる。
     */
    virtual void current(jc_sp<EGLContextProtocol> context, jc_sp<EGLSurfaceProtocol> surface) = 0;

    /**
     * 描画用のディスプレイにバックバッファの内容を転送する。
     * 転送方法については各デバイスの実装に従う。
     * このメソッドは完了するまでブロックされることを保証しない（デバイス依存、基本的にブロッキングになるように調整している）
     */
    virtual jcboolean postFrontBuffer(jc_sp<EGLSurfaceProtocol> displaySurface) = 0;

    /**
     * 握っているEGL資源を明示的に開放する。
     */
    virtual void dispose() = 0;
};

    /**
     * Native定義を取得するプロトコル
     */
typedef jc_sp<EGLProtocol> MEGLProtocol;

}
}

#endif /* JCEGL_H_ */
