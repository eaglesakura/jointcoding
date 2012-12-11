/*
 * jcglGPULock.h
 *
 *  Created on: 2012/09/20
 */

#ifndef JCGLGPULOCK_H_
#define JCGLGPULOCK_H_

#include    "jcglDevice.h"

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
     * デバイスの専有を行う
     */
    void lockDevice() {
        if(!device) {
            locked = jcfalse;
            return;
        }

        // 現在のスレッドが専有状態にあるかをチェックする
        const jcboolean current_thread = device->isCurrentThread();

        // 専有が現在のスレッドと違うのならば、ロックリクエストを送る
        if(!current_thread) {
            device->incLockRequest();
        }

        // まずはロックを占有権を取得する
        lock.reset(new MutexLock(device->getGPUMutex()));

        // ロックが完了したので、現在のスレッドと違っていたならばロックリクエスト数を解放する
        if( !current_thread ) {
            device->decLockRequest();
        }

// contextとthreadを結びつける
        locked = device->makeCurrent(EGLMakeCurrent_Bind);

// ロックに失敗したら、専有も解除する
        if( !locked ) {
            lock.reset();
        }
    }

    /**
     * デバイスの専有を解除する
     */
    void unlockDevice() {
        if(locked) {
            device->makeCurrent(EGLMakeCurrent_Unbind);
        }

        lock.reset();
    }

    /**
     * new/deleteも許可しない
     */
    static void* operator new(size_t size);

    /**
     * new/deleteも許可しない
     */
    static void operator delete(void* ptr);
public:

    /**
     *
     * @param device ロック対象のデバイス。DeviceLockが生きている限り、deviceの利用は当該スレッドの専有となる。
     * @param throw_error エラーが発生した場合、例外を投げるならtrue。デフォルトがtrueなのは、チェック機構予備忘れ防止の為。
     */
    DeviceLock(MDevice device, jcboolean throw_error) {
        this->locked = jcfalse;
        this->device = device;
        lockDevice();

        if(throw_error) {
            if(!isLockCompleted()) {
                throw create_exception_t(EGLException, EGLException_ContextAttachFailed);
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
