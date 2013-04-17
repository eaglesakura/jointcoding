/*
 * JCSmartJObject.h
 *
 *  Created on: 2012/06/09
 *      Author: Takeshi
 */

#ifndef JCSMARTJOBJECT_H_
#define JCSMARTJOBJECT_H_

#include    "jointcoding-android.h"
#include    "jcandroid/ndk-support.h"
#include    "jni.h"

// #define JOBJECT_LOG_OUT

namespace ndk {

inline jobject change_globalref(JNIEnv *env, jobject obj) {
    jobject old_ref = obj;
    jobject new_ref = env->NewGlobalRef(obj);

    const jobjectRefType refType = env->GetObjectRefType(obj);
    if (refType == JNILocalRefType) {
        env->DeleteLocalRef(old_ref);
    }

    return new_ref;
}

/**
 * Javaのリファレンス管理を行う
 * * SmartJObject<T>はデストラクタ実行時にDeleteGlobalRef/LocalRefを行うため、明示的な解放は不要である
 */
template<typename T>
class SmartJObject: public Object {
protected:
    /**
     * 管理しているオブジェクト
     */
    T obj;

    /**
     * ブリッジのための拡張データ
     */
    jc_selp<Object> extra;

    /**
     * 読み込み用のクラス
     */
    jclass clazz;

    /**
     * Object#toString()用のメソッド
     */
    jmethodID method_toString;

    /**
     * Object#hashcode()用のメソッド
     */
    jmethodID method_hashCode;

    /**
     * Object#equals()用のメソッド
     */
    jmethodID method_equals;

    /**
     * このオブジェクトをグローバル参照していたらtrue
     */
    jcboolean globalRef;

    /**
     * グローバル参照している場合、オーナーシップを破棄する
     */
    virtual void release() {
        CALL_JNIENV();
        if (globalRef) {
#ifdef JOBJECT_LOG_OUT
            jclogf("delete globalRef(%x)", obj);
#endif
            env->DeleteGlobalRef(obj);
            env->DeleteGlobalRef(clazz);
            globalRef = jcfalse;
        } else {
            if(obj) {
                env->DeleteLocalRef(obj);
            }
            if( clazz ) {
                env->DeleteLocalRef(clazz);
            }
        }

        this->obj = NULL;
        this->clazz = NULL;
    }

public:
    /**
     *
     */
    SmartJObject(T jobj = NULL) {
        method_toString = NULL;
        method_hashCode = NULL;
        globalRef = jcfalse;
        method_equals = NULL;
        clazz = NULL;
        obj = NULL;

        this->reset(jobj);
    }

    /**
     *
     */
    SmartJObject(const SmartJObject<T> &cpy) {
        method_toString = NULL;
        method_hashCode = NULL;
        globalRef = jcfalse;
        method_equals = NULL;
        clazz = NULL;
        obj = NULL;

        this->reset(cpy.obj);
        if (cpy.globalRef) {
            this->addGlobalRef();
        }
    }

    /**
     * 保持しているオブジェクトの廃棄を行う
     */
    virtual ~SmartJObject() {
        this->release(); // 参照を持っていたら破棄を行う
    }

    /**
     * クラスを取得する
     */
    inline jclass getClass() const {
        return clazz;
    }

    /**
     * 管理しているオブジェクトを取得する
     */
    inline jobject getObject() const {
        return obj;
    }

    /**
     * 文字列に変換する
     */
    virtual jstring toString() const {
        CALL_JNIENV();
        return (jstring) env->CallObjectMethod(getObject(), method_toString, NULL);
    }

    /**
     * ハッシュコードを計算する
     */
    virtual s32 hashcode() const {
        CALL_JNIENV();
        return env->CallIntMethod(getObject(), method_toString, NULL);
    }

    /**
     * 同一性を検証する
     */
    virtual jcboolean equals(jobject obj) const {
        CALL_JNIENV();
        return env->CallBooleanMethod(obj, method_equals, obj);
    }

    /**
     * グローバル参照を持っている場合はtrueを返す。
     */
    virtual jcboolean hasGlobalRef() const {
        return globalRef;
    }

    /**
     * グローバル参照として管理を行う。
     */
    virtual SmartJObject<T>* addGlobalRef() {
        if (!globalRef) {
            CALL_JNIENV();
            obj = (T)change_globalref( env, obj );
            clazz = (jclass) change_globalref(env,(jobject) clazz);
            globalRef = true;
#ifdef JOBJECT_LOG_OUT
            jclogf("add globalRef(%x)", obj);
#endif
        }
        return this;
    }

    SmartJObject<T>& operator=(const SmartJObject<T> &cpy) {
        reset(cpy.obj);

        // globalRef属性があるなら、こちらもglobalしてあげる。
        if( cpy.globalRef) {
            this->addGlobalRef();
        }

        return *this;
    }

    SmartJObject<T>& operator=(const T obj) {
        reset(obj);
        return (*this);
    }

    SmartJObject<T>& reset( const T jobj ) {
        this->release();

        globalRef = jcfalse;
        this->obj = jobj;

        // クラスの取得
        if(jobj) {
            CALL_JNIENV();
            clazz = env->GetObjectClass(getObject());
            // 基本メソッドの生成
            {
                method_toString = env->GetMethodID(clazz, "toString", "()Ljava/lang/String;");
                method_hashCode = env->GetMethodID(clazz, "hashCode", "()I");
                method_equals = env->GetMethodID(clazz, "equals", "(Ljava/lang/Object;)Z");
            }
        } else {
            method_equals = NULL;
            method_hashCode = NULL;
            method_toString = NULL;
        }
        return (*this);
    }

    SmartJObject<T>& reset(const T jobj, jcboolean globalRef) {
        reset(jobj);

        if(globalRef) {
            addGlobalRef();
        }
        return (*this);
    }

    /**
     * 拡張データを取得する
     */
    jc_selp<Object> getExtraData( ) const {
        return extra;
    }

    /**
     * ダウンキャストして拡張データを取得する
     */
    template<typename DC>
    jc_selp<DC> getExtraDataTo() const {
        return extra.downcast<DC>();
    }

    /**
     * 拡張データを設定する
     */
    void setExtraData( const jc_selp<Object> extra) {
        this->extra = extra;
    }
};

/**
 * jclassのラッパ
 */
typedef SmartJObject<jobject> JObject;

/**
 * jstringのラッパ
 */
typedef SmartJObject<jstring> JString;

}

#endif /* JCSMARTJOBJECT_H_ */
