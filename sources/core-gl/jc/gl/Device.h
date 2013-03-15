/*
 * jcglDevice.h
 *
 *  Created on: 2012/08/23
 */

#ifndef JCGLDEVICE_H_
#define JCGLDEVICE_H_

#include    "jc/egl/EGL.h"
#include    "jc/egl/EGLContext.h"
#include    "jc/egl/EGLSurface.h"
#include    "jc/thread/Thread.h"
#include    "jc/thread/Mutex.h"

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
 * ロックリクエストのID
 */
typedef u64 request_id;

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
     * ロックリクエストを送るためのMutex
     */
    jcmutex request_mutex;

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
     * 現在のリクエストID
     * リクエストの予約番号として処理される。
     */
    request_id current_request;

    /**
     * 現在占有権のあるリクエストID
     * DeviceLockが開放される度にデクリメントされる
     */
    request_id current_id;

private:
    // for DeviceLock

    /**
     * ロックリクエストを発行する。
     *
     */
    virtual request_id lockRequest() {
        MutexLock lock(request_mutex);
        return current_request++;
    }

    /**
     * リクエストが占有権を持っている場合はtrueを返す
     */
    virtual request_id isCurrentRequest(const request_id id) {
        MutexLock lock(request_mutex);
        assert(current_id <= id);
        return current_id == id;
    }

    /**
     * リクエストの処理を終了した
     */
    virtual void unlockRequest(const request_id id) {
        MutexLock lock(request_mutex);
        assert(id == current_id);
        ++current_id;
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
        MThreadID id = threadId;

        // threadがそもそも設定されていないならfalseしかない
        if (!id) {
            return jcfalse;
        }

        return id->isCurrent();
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

    /**
     * サーフェイスのアスペクト比を取得する
     */
    virtual float getSurfaceAspect() const {
        return (float) surface->getWidth() / (float) surface->getHeight();
    }

    /**
     * 左上起点のディスプレイ座標系から、左下起点のウィンドウ座標系へ変換する。
     *
     * resultのポインタをそのまま帰す。
     */
    virtual void convertViewportRect(const RectI &displayRect, s32 *result_x, s32 *result_y, s32 *result_width, s32 *result_height) const {
        const s32 display_width = surface->getWidth();
        const u32 display_height = surface->getHeight();

        const u32 viewport_x = displayRect.left;
        const u32 viewport_y = display_height - displayRect.bottom;
        const u32 viewport_width = displayRect.width();
        const u32 viewport_height = displayRect.height();

        if (result_x) {
            (*result_x) = viewport_x;
        }
        if (result_y) {
            (*result_y) = viewport_y;
        }
        if (result_width) {
            (*result_width) = viewport_width;
        }
        if (result_height) {
            (*result_height) = viewport_height;
        }
    }

    /**
     * 呼び出しスレッドがリクエスト権を持てるようになるまで待つ。
     * 但し、その他のスレッドからの要求で呼び出し順が変化ある場合は何もしない。
     */
    virtual void waitLockRequest(const s32 sleepMs, const jcboolean *canceled) {
        while (current_request != current_id) {
            if (canceled && (*canceled)) {
                // キャンセルチェック
                return;
            }
            Thread::sleep(sleepMs);
        }
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
