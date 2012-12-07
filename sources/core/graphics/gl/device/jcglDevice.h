/*
 * jcglDevice.h
 *
 *  Created on: 2012/08/23
 */

#ifndef JCGLDEVICE_H_
#define JCGLDEVICE_H_

#include    "jcEGL.h"
#include    "jcglEGLContext.h"
#include    "jcglEGLSurface.h"
#include    "jcThread.h"

namespace jc {
namespace gl {

enum EGLMakeCurrent_e {
    /**
     * スレッドとEGLを関連付け、このスレッドでOpenGLコマンドを呼び出せるようにする
     */
    EGLMakeCurrent_Bind,

    /**
     * スレッドとEGLの関連付けを解除し、他のスレッドへOpenGLコマンドを解放する。
     */
    EGLMakeCurrent_Unbind,
};

/**
 * レンダリングデバイスに与えるフラグ情報。
 */
enum DeviceFlag_e {

    /**
     * 廃棄リクエストが行われた。
     * 速やかにレンダリング排除を行わなければならない。
     */
    DeviceFlag_RequestDestroy = 0x1 << 0,
};

class DeviceLock;

/**
 * 描画に必要な情報を一塊にしたものをDeviceとする。
 * Deviceはアプリ内で複数個作れる。
 * Deviceの作成方法は各プラットフォームに依存する。
 */
class Device: public Object {
protected:
    /**
     * 関連付けられたEGLインターフェース。
     * 多くの場合、内部はシングルトン管理されている。
     */
    MEGLProtocol egl;

    /**
     * 関連付けられたEGLコンテキスト。
     * 基本的に１つの描画ターゲットごとに１つを割り当てることを推奨。
     * その他、アプリによって作り方は適宜変えること。
     */
    MEGLContextProtocol context;

    /**
     * 描画用に関連付けられたサーフェイス
     */
    MEGLSurfaceProtocol surface;

    /**
     * デバイスに付与されたフラグ
     */
    u32 flags;

    /**
     * GPU制御のためのMutex。
     * ひとつのContext(eglMakeCurrent)に対して１つ割り当てる。
     * eglMakeCurrent中はこのMutexを取得し、安全にロック制御を行う必要がある。
     */
    jcmutex gpuMutex;

    /**
     * バインドされているスレッド
     */
    MThreadID threadId;

    /**
     * 別スレッドからのロックリクエスト数
     */
    s32 lock_request;

private:
    // for DeviceLock

    /**
     * ロックリクエスト数をインクリメントする。
     * DeviceLockを得る直前にインクリメントされる。
     */
    virtual void incLockRequest() {
        ++lock_request;
    }

    /**
     * ロックリクエスト数をデクリメントする。
     * DeviceLockを得た直後にデクリメントされる。
     */
    virtual void decLockRequest() {
        --lock_request;
    }

    /**
     * ロックリクエストを処理しきれていないならtrueを返す。
     * 短い間に同一スレッドからのMutexは制御がわたらない危険性がある。（例えば、while(true){ rendering(); }のレンダリングループ）
     * 短い間にロックを再取得する恐れがある場合は適宜ロックリクエストのチェックを行う。
     */
    virtual jcboolean hasLockRequest() {
        return lock_request > 0;
    }

    /**
     * EGLの関連付けを設定し直す。
     */
    jcboolean makeCurrent(EGLMakeCurrent_e type);

    /**
     * Deviceがカレント設定されているスレッドIDを取得する。
     */
    inline MThreadID getCurrentThreadId() {
        return threadId;
    }
public:
    Device();
    virtual ~Device();

    /**
     * EGLを設定する
     */
    virtual void setEGL(MEGLProtocol egl) {
        this->egl = egl;
    }

    /**
     * コンテキストを設定する
     */
    virtual void setContext(MEGLContextProtocol context) {
        this->context = context;
    }

    /**
     * サーフェイスを設定する
     */
    virtual void setSurface(MEGLSurfaceProtocol surface) {
        this->surface = surface;
    }
    /**
     * EGL本体を取得する
     */
    virtual MEGLProtocol getEGL() const {
        return egl;
    }

    /**
     * EGLコンテキストを取得する
     * コンテキストはGLのステートを保持し、効率的なステートアクセスのためのクラスを提供する。
     */
    virtual MEGLContextProtocol getContext() const {
        return context;
    }

    /**
     * レンダリング用サーフェイスを取得する。
     */
    virtual MEGLSurfaceProtocol getSurface() const {
        return surface;
    }

    /**
     * フロントバッファへサーフェイスを転送する。
     * DeviceLockを得ている必要がある。
     */
    virtual jcboolean postFrontBuffer();

    /**
     * フラグが設定されている場合、trueを返す。
     */
    virtual jcboolean hasFlags(const u32 check_flags) {
        return has_flag_all(flags, check_flags);
    }
    /**
     * 管理しているリソースを解放する。
     */
    virtual void dispose();

    /**
     * GPU制御用のMutexを取得する。
     */
    virtual jcmutex& getGPUMutex() {
        return gpuMutex;
    }

    /**
     * 管理フラグを追加する。
     */
    virtual void addFlags(const u32 flags_bits) {
        flags |= flags_bits;
    }

    /**
     * 管理フラグを削除する。
     */
    virtual void removeFlags(const u32 flags_bits) {
        flags = (flags & (~flags_bits));
    }

    /**
     * 呼び出しをされているのがカレントスレッドに当たる場合はtrueを返す。
     */
    virtual jcboolean isCurrentThread() {

        // threadがそもそも設定されていないならfalseしかない
        if (!threadId) {
            return jcfalse;
        }

        return threadId->isCurrent();
    }

    /**
     * ロックリクエストを処理しきるまで、ウェイトをかける。
     */
    virtual void waitLockRequest(s32 sleep_ms, jcboolean *cancel_flag) {
        // ロックリクエストを保持している間はループさせる
        while (hasLockRequest()) {
            Thread::sleep(sleep_ms);

            if (cancel_flag) {
                // キャンセルフラグが設定されていて、かつjctrueである場合はキャンセルを行う。
                if (*cancel_flag) {
                    return;
                }
            }
        }
    }

    /**
     * 現在のコンテキストのステートを取得する
     */
    virtual MGLState getState() {
        return context->getState();
    }

    /**
     * 現在のコンテキストのVRAMを取得する。
     */
    virtual VRAM getVRAM() {
        return context->getVRAM();
    }

    friend class DeviceLock;
};

/**
 * managed
 */
typedef jc_sp<Device> MDevice;

}
}

#endif /* JCGLDEVICE_H_ */
