/*
 * JCJniWrapper.cpp
 *
 *  Created on: 2012/06/12
 *      Author: Takeshi
 */

#include "jointcoding-android.h"
#include "jcException.h"

namespace ndk {

/**
 *
 */
JniWrapper::JniWrapper(jobject obj) :
        ndk::SmartJObject<jobject>(obj) {
}

/**
 * メソッドを読み込む。
 */
jmethodID JniWrapper::loadMethod(jclass clazz, const charactor* name, const charactor* signeture, jcboolean isStatic) {
    CALL_JNIENV();

    jmethodID result = NULL;
    if (isStatic) {
        result = env->GetStaticMethodID(clazz, name, signeture);
    } else {
        result = env->GetMethodID(clazz, name, signeture);
    }

    // 読み込んだメソッドIDを出力
    if (result == NULL) {
        jclogf("Method Load Error !! Name %s = id %d", name, (s32)result);
    }

    return result;
}

/**
 * 例外を取得する
 */
jc_sp<JniException> JniWrapper::popException() {
    CALL_JNIENV();
    jthrowable throwable = env->ExceptionOccurred();
    if (throwable) {
        env->ExceptionClear();
    }
    jc_sp<JniException> managed = JniException::global(throwable);
    return managed;
}

/**
 * 例外が発生していたら例外を投げる。
 * 例外を取得する。
 * exceptionがNULLの場合、popのみを行う。
 */
void JniWrapper::throwHasException() {

    CALL_JNIENV();
    jthrowable throwable = env->ExceptionOccurred();
    if (throwable) {
        env->ExceptionClear();
        env->DeleteLocalRef(throwable);

        throw create_exception(JavaMethodException, "throwHasException()");
    }
}

}

