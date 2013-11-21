/*
 * jcglGPULock.h
 *
 *  Created on: 2012/09/20
 */

#ifndef JCGLGPULOCK_H_
#define JCGLGPULOCK_H_

#include    "jc/mem/SmartPtr.h"
#include    "jc/gl/gpu/Device.h"

namespace jc {
namespace gl {

/**
 * GPU専用のロックオブジェクトを作成する。
 * ロック中のみレンダリングやテクスチャアクションが可能になる。
 */
class DeviceLock {
    /**
     * コピー不可能
     */
    DeviceLock(const DeviceLock& origin);

    /**
     * 制御用のデバイス
     */
    MDevice device;

    /**
     * 制御用のロックオブジェクト
     */
    jc_sp<MutexLock> lock;

    /**
     * ロックが正常に完了したらtrue
     */
    jcboolean locked;

    /**
     * 既にロックされた状態の場合
     */
    jcboolean def_locked;

    /**
     * eglMakeCurrentの要求を行う場合はtrue
     * default=true
     */
    jcboolean makeCurrent;

    /**
     * ロックリクエスト整理番号
     */
    request_id lock_request;

    /**
     * ロックリクエストをこのインスタンスが行った場合はtrue
     */
    jcboolean lock_requested;

    /**
     * デバイスの専有を行う
     */
    void lockDevice() {
        if(!device) {
            jclog("device not found");
            locked = jcfalse;
            lock.reset();
            return;
        }

        // 現在のスレッドが専有状態にあるかをチェックする
        const jcboolean current_thread = device->isCurrentThread();

        // 専有が現在のスレッドと違うのならば、ロックリクエストを送る
        if(!current_thread) {
            lock_request = device->lockRequest();
            lock_requested = jctrue;

            while(!device->isCurrentRequest(lock_request)) {
//                jclogf("wait lock request(%d)", (s32)lock_request);
                Thread::sleep(1);
            }
            // Mutex占有権を取得する
            lock.reset(mark_new MutexLock(device->getGPUMutex()));
        } else {
            def_locked = jctrue;
        }

        // contextとthreadを結びつける
        if(current_thread) {
            locked = jctrue;
        } else {
            if(makeCurrent) {
                locked = device->makeCurrent(EGLMakeCurrent_Bind);
            } else {
                locked = jctrue;
            }
        }

        // ロックに失敗したら、専有も解除する
        if( !locked ) {
            jclog("lock failed");
            lock.reset();
        }
    }

    /**
     * デバイスの専有を解除する
     */
    void unlockDevice() {
        if(locked && !def_locked) {
            if(makeCurrent) {
                device->makeCurrent(EGLMakeCurrent_Unbind);
            }
        }

        if(lock_requested) {
            device->unlockRequest(lock_request);
        }
        lock.reset();
    }
public:

    /**
     *
     * @param device ロック対象のデバイス。DeviceLockが生きている限り、deviceの利用は当該スレッドの専有となる。
     * @param throw_error エラーが発生した場合、例外を投げるならtrue。デフォルトがtrueなのは、チェック機構予備忘れ防止の為。
     */
    DeviceLock(MDevice device, const jcboolean throw_error, const jcboolean makeCurrent = jctrue) {
        this->locked = jcfalse;
        this->def_locked = jcfalse;
        this->device = device;
        this->lock_request = 0;
        this->lock_requested = jcfalse;
        this->makeCurrent = makeCurrent;
        lockDevice();

        if(throw_error) {
            if(!isLockCompleted()) {
                if(device->hasFlags(DeviceFlag_RequestDestroy)) {
                    throw create_exception_mt(EGLException, EGLException_DeviceHasDestroyFlag, "Abort Rendering...");
                } else {
                    throw create_exception_t(EGLException, EGLException_ContextAttachFailed);
                }
            }
        }
    }

    ~DeviceLock() {
        unlockDevice();
    }

    /**
     * 速やかにロックを外して欲しいというリクエストを受けている。
     */
    jcboolean hasAbortRequest() {
        return device->hasFlags(DeviceFlag_RequestDestroy);
    }

    /**
     * ロックに失敗したらtrueを返す
     */
    jcboolean isLockCompleted( ) {
        return locked;
    }
};

}
}

#endif /* JCGLGPULOCK_H_ */
