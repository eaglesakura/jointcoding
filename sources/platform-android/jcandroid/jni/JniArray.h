/*
 * JniArray.h
 *
 *  Created on: 2012/06/13
 *      Author: Takeshi
 */

#ifndef JniArray_H_
#define JniArray_H_

#include "jointcoding-android.h"

namespace ndk {

/**
 * 配列解放時のオプション
 */
enum JniArrayUnlock_e {
    /**
     * 配列をJava側に反映する
     */
    JniArrayUnlock_Commit = JNI_COMMIT,

    /**
     * 編集結果を反映しない
     */
    JniArrayUnlock_Abort = JNI_ABORT,
};

/**
 * 配列管理用
 */
template<typename ArrayType, typename PrimitiveType>
class JniArray: public SmartJObject<jarray> {
protected:
    /**
     * 長さは一定のため、事前に保存しておく
     */
    s32 _length;

    /**
     * 編集用のポインタ。
     * lock()中のみ値が入る
     */
    PrimitiveType *pointer;
    JniArray(ArrayType array) :
            SmartJObject<jarray>(array) {
        CALL_JNIENV();
        _length = env->GetArrayLength(array);
        pointer = NULL;
    }
public:
    virtual ~JniArray() {
        assert(pointer == NULL);
    }

    /**
     * 編集用のポインタを取得する
     * lock()中のみ有効となる
     */
    virtual PrimitiveType* getPointer() {
        return pointer;
    }

    /**
     * 配列オブジェクトを取得する
     */
    inline ArrayType getArrayObject() {
        return (ArrayType) getObject();
    }

    /**
     * 配列の内容をdstへコピーする
     */
    virtual void get(s32 srcIndex, s32 length, PrimitiveType *dst) = 0;

    /**
     * srcの内容を配列にコピーする
     */
    virtual void set(s32 dstIndex, s32 length, PrimitiveType *src) = 0;

    /**
     * 特定の内容を設定する
     */
    virtual void set(s32 index, PrimitiveType value) {
        set(index, 1, &value);
    }

    /**
     * 編集を開始する
     */
    virtual PrimitiveType* lock() = 0;

    /**
     * 編集を終了する
     */
    virtual void unlock(JniArrayUnlock_e type) = 0;

    /**
     * 配列の長さを取得する
     */
    inline s32 length() const {
        return _length;
    }

    /**
     * 特定の内容を取得する
     */
    inline PrimitiveType getFrom(s32 index) {
        PrimitiveType result = (PrimitiveType) 0;
        get(index, 1, &result);
        return result;
    }

};

/**
 * Byte配列
 */
#define PRIMITIVE_TYPE jbyte
class JByteArray: public JniArray<jbyteArray, PRIMITIVE_TYPE> {
    JByteArray(jbyteArray array) :
            JniArray<jbyteArray, PRIMITIVE_TYPE>(array) {
    }
public:
    virtual ~JByteArray() {

    }

    /**
     * 配列の内容をdstへコピーする
     */
    virtual void get(s32 srcIndex, s32 length, PRIMITIVE_TYPE *dst) {
        CALL_JNIENV();
        env->GetByteArrayRegion(getArrayObject(), srcIndex, length, dst);
    }

    /**
     * srcの内容を配列にコピーする
     */
    virtual void set(s32 dstIndex, s32 length, PRIMITIVE_TYPE *src) {
        CALL_JNIENV();
        env->SetByteArrayRegion(getArrayObject(), dstIndex, length, src);
    }

    /**
     * 編集を開始する
     */
    virtual PRIMITIVE_TYPE* lock() {
        if (pointer) {
            return pointer;
        }

        CALL_JNIENV();
        pointer = env->GetByteArrayElements(getArrayObject(), NULL);
        return pointer;
    }

    /**
     * 編集を終了する
     */
    virtual void unlock(JniArrayUnlock_e type) {
        if (!pointer) {
            return;
        }

        CALL_JNIENV();
        env->ReleaseByteArrayElements(getArrayObject(), pointer, (jint) type);
        pointer = NULL;
    }

    static jc_sp<JByteArray> wrap(jbyteArray array) {
        return jc_sp<JByteArray>( new JByteArray(array));
    }

    static jc_sp<JByteArray> global(jbyteArray array) {
        return jc_sp<JByteArray>( (JByteArray*) (new JByteArray(array))->addGlobalRef());
    }
};
#undef PRIMITIVE_TYPE

/**
 * Int配列
 */
#define PRIMITIVE_TYPE jint
class JIntArray: public JniArray<jintArray, PRIMITIVE_TYPE> {
    JIntArray(jintArray array) :
            JniArray<jintArray, PRIMITIVE_TYPE>(array) {
    }
public:
    virtual ~JIntArray() {

    }

    /**
     * 配列の内容をdstへコピーする
     */
    virtual void get(s32 srcIndex, s32 length, PRIMITIVE_TYPE *dst) {
        CALL_JNIENV();
        env->GetIntArrayRegion(getArrayObject(), srcIndex, length, dst);
    }

    /**
     * srcの内容を配列にコピーする
     */
    virtual void set(s32 dstIndex, s32 length, PRIMITIVE_TYPE *src) {
        CALL_JNIENV();
        env->SetIntArrayRegion(getArrayObject(), dstIndex, length, src);
    }

    /**
     * 編集を開始する
     */
    virtual PRIMITIVE_TYPE* lock() {
        if (pointer) {
            return pointer;
        }

        CALL_JNIENV();
        pointer = env->GetIntArrayElements(getArrayObject(), NULL);
        return pointer;
    }

    /**
     * 編集を終了する
     */
    virtual void unlock(JniArrayUnlock_e type) {
        if (!pointer) {
            return;
        }

        CALL_JNIENV();
        env->ReleaseIntArrayElements(getArrayObject(), pointer, (jint) type);
        pointer = NULL;
    }

    static jc_sp<JIntArray> wrap(jintArray array) {
        return jc_sp<JIntArray>(new JIntArray(array));
    }

    static jc_sp<JIntArray> global(jintArray array) {
        return jc_sp<JIntArray>( (JIntArray*) (new JIntArray(array))->addGlobalRef() );
    }
};
#undef PRIMITIVE_TYPE

/**
 * float配列
 */
#define PRIMITIVE_TYPE jfloat
class JFloatArray: public JniArray<jfloatArray, PRIMITIVE_TYPE> {
    JFloatArray(jfloatArray array) :
            JniArray<jfloatArray, PRIMITIVE_TYPE>(array) {
    }
public:
    virtual ~JFloatArray() {

    }

    /**
     * 配列の内容をdstへコピーする
     */
    virtual void get(s32 srcIndex, s32 length, PRIMITIVE_TYPE *dst) {
        CALL_JNIENV();
        env->GetFloatArrayRegion(getArrayObject(), srcIndex, length, dst);
    }

    /**
     * srcの内容を配列にコピーする
     */
    virtual void set(s32 dstIndex, s32 length, PRIMITIVE_TYPE *src) {
        CALL_JNIENV();
        env->SetFloatArrayRegion(getArrayObject(), dstIndex, length, src);
    }

    /**
     * 編集を開始する
     */
    virtual PRIMITIVE_TYPE* lock() {
        if (pointer) {
            return pointer;
        }

        CALL_JNIENV();
        pointer = env->GetFloatArrayElements(getArrayObject(), NULL);
        return pointer;
    }

    /**
     * 編集を終了する
     */
    virtual void unlock(JniArrayUnlock_e type) {
        if (!pointer) {
            return;
        }

        CALL_JNIENV();
        env->ReleaseFloatArrayElements(getArrayObject(), pointer, (jint) type);
        pointer = NULL;
    }

    static jc_sp<JFloatArray> wrap(jfloatArray array) {
        return jc_sp<JFloatArray>( new JFloatArray(array) );
    }

    static jc_sp<JFloatArray> global(jfloatArray array) {
        return jc_sp<JFloatArray>((JFloatArray*) (new JFloatArray(array))->addGlobalRef());
    }
};
#undef PRIMITIVE_TYPE

/**
 * float配列
 */
#define PRIMITIVE_TYPE jdouble
class JDoubleArray: public JniArray<jdoubleArray, PRIMITIVE_TYPE> {
    JDoubleArray(jdoubleArray array) :
            JniArray<jdoubleArray, PRIMITIVE_TYPE>(array) {
    }

public:
    virtual ~JDoubleArray() {

    }
    /**
     * 配列の内容をdstへコピーする
     */
    virtual void get(s32 srcIndex, s32 length, PRIMITIVE_TYPE *dst) {
        CALL_JNIENV();
        env->GetDoubleArrayRegion(getArrayObject(), srcIndex, length, dst);
    }

    /**
     * srcの内容を配列にコピーする
     */
    virtual void set(s32 dstIndex, s32 length, PRIMITIVE_TYPE *src) {
        CALL_JNIENV();
        env->SetDoubleArrayRegion(getArrayObject(), dstIndex, length, src);
    }

    /**
     * 編集を開始する
     */
    virtual PRIMITIVE_TYPE* lock() {
        if (pointer) {
            return pointer;
        }

        CALL_JNIENV();
        pointer = env->GetDoubleArrayElements(getArrayObject(), NULL);
        return pointer;
    }

    /**
     * 編集を終了する
     */
    virtual void unlock(JniArrayUnlock_e type) {
        if (!pointer) {
            return;
        }

        CALL_JNIENV();
        env->ReleaseDoubleArrayElements(getArrayObject(), pointer, (jint) type);
        pointer = NULL;
    }

    static jc_sp<JDoubleArray> wrap(jdoubleArray array) {
        return jc_sp<JDoubleArray>(new JDoubleArray(array));
    }

    static jc_sp<JDoubleArray> global(jdoubleArray array) {
        return jc_sp<JDoubleArray>((JDoubleArray*) (new JDoubleArray(array))->addGlobalRef());
    }
};
#undef PRIMITIVE_TYPE

/**
 * object配列
 */
#define PRIMITIVE_TYPE jobject
class JObjectArray: public JniArray<jobjectArray, PRIMITIVE_TYPE> {
    JObjectArray(jobjectArray array) :
            JniArray<jobjectArray, PRIMITIVE_TYPE>(array) {
    }

public:
    virtual ~JObjectArray() {

    }
    /**
     * 配列の内容をdstへコピーする
     */
    virtual void get(s32 srcIndex, s32 length, PRIMITIVE_TYPE *dst) {
        CALL_JNIENV();
        for (s32 i = 0; i < length; ++i) {
            dst[i] = env->GetObjectArrayElement(getArrayObject(), srcIndex + i);
        }
    }

    /**
     * 配列オブジェクトを取得する
     */
    virtual jobject get(const s32 index) {
        // 長さをチェックする
        assert(index < length());

        CALL_JNIENV();
        return env->GetObjectArrayElement(getArrayObject(), index);
    }

    /**
     * srcの内容を配列にコピーする
     */
    virtual void set(s32 dstIndex, s32 length, PRIMITIVE_TYPE *src) {
        CALL_JNIENV();
        for( s32 i = 0; i < length; ++i ) {
            env->SetObjectArrayElement(getArrayObject(), dstIndex + i, src[i]);
        }
    }

    /**
     * 編集を開始する
     */
    virtual PRIMITIVE_TYPE* lock() {
        assert(false);

        // 呼び出しは必ず失敗する
        return NULL;
    }

    /**
     * 編集を終了する
     */
    virtual void unlock(JniArrayUnlock_e type) {
        assert(false);

        // 呼び出しは必ず失敗する
    }

    static jc_sp<JObjectArray> wrap(jobjectArray array) {
        return jc_sp<JObjectArray>(new JObjectArray(array));
    }

    static jc_sp<JObjectArray> global(jobjectArray array) {
        return jc_sp<JObjectArray>((JObjectArray*) (new JObjectArray(array))->addGlobalRef());
    }
};
#undef PRIMITIVE_TYPE

}

#endif /* JniArray_H_ */
