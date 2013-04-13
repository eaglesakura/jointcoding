/*
 * Jointable.h
 *
 *  Created on: 2013/04/13
 */

#ifndef JOINTABLE_H_
#define JOINTABLE_H_

namespace ndk {

/**
 * void*を設定する
 */
extern void setJointContextPointer_void(jobject jJointable, const s32 key, const void* pointer);

/**
 * Object*を設定する
 */
extern void setJointContextPointer_Object(jobject jJointable, const s32 key, const Object* pointer);

/**
 * jc_sp<Object>*を設定する
 */
extern void setJointContextPointer_SharedObject(jobject jJointable, const s32 key, const jc_sp<Object>* pointer);


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
