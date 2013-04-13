/*
 * Jointable.h
 *
 *  Created on: 2013/04/13
 */

#ifndef JOINTABLE_H_
#define JOINTABLE_H_

namespace ndk {


/**
 * NDK接続用のクラスを取得する
 *
 * @parma jJointable Jointableインターフェースを実装したクラスのインスタンス
 */
template<typename T>
T* getJointContextPointer(const jobject jJointable, const s32 key) {
    extern void* getJointContextPointer_void(const jobject, const s32);
    return static_cast<T*>(getJointContextPointer_void(jJointable, key));
}

}

#endif /* JOINTABLE_H_ */
