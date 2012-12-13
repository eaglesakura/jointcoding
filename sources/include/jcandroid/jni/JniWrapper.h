/*
 * JCJniWrapper.h
 *
 *  Created on: 2012/06/08
 *      Author: Takeshi
 */

#ifndef JCJNIWRAPPER_H_
#define JCJNIWRAPPER_H_

#include    "jointcoding-android.h"
#include    "jcandroid/ndk-support.h"
namespace ndk {

class JniException;

/**
 * JNIのラッピングを行う。
 * このオブジェクトを継承するクラスは、基本的にAnnotationを利用して自動的に生成される。
 */
class JniWrapper: public SmartJObject<jobject> {
protected:
    /**
     *
     */
    JniWrapper(jobject obj);

public:
    virtual ~JniWrapper() {
    }

    /**
     * 例外を取得する
     * Exceptionはグローバル参照されていることに注意すること。
     */
    virtual jc_sp<JniException> popException();

    /**
     * 例外が発生中の場合はtrue
     */
    virtual jcboolean hasException() {
        CALL_JNIENV();
        return env->ExceptionCheck();
    }

    /**
     * 例外が発生していたら例外を投げる。
     */
    virtual void throwHasException();

    /**
     * 例外を投げる。
     */
    virtual void throwException(jclass clazz, const charactor* message) {
        CALL_JNIENV();
        env->ThrowNew(clazz, message);
    }

    /**
     * メソッドを読み込む。
     */
    static jmethodID loadMethod(jclass clazz, const charactor* name, const charactor* signeture, jcboolean isStatic);
};

}

#endif /* JCJNIWRAPPER_H_ */
