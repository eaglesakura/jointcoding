package com.eaglesakura.jc.resource.jni;

import java.lang.ref.WeakReference;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * JniからJavaオブジェクトの生存を確認するための監視オブジェクト。
 * 監視したい対象を弱参照保持して、相手の生存期間が終了していたら知らせる。
 * 
 *
 */
@JCClass(
         cppNamespace = "ndk")
public class ObjMonitoring {
    /**
     * 監視対象のオブジェクト
     */
    WeakReference<Object> target = null;

    /**
     * 監視対象を指定して作成する。
     * @param target
     */
    public ObjMonitoring(Object target) {
        this.target = new WeakReference<Object>(target);
    }

    /**
     * 監視対象が生存している場合はtrueを返す。
     * @return
     */
    @JCMethod
    public boolean exist() {
        if (target == null) {
            return false;
        }

        return target.get() != null;
    }

    /**
     * 監視対象のオブジェクトを取得する。
     * @return
     */
    public Object get() {
        return target != null ? target.get() : null;
    }

    /**
     * 新たな監視オブジェクトを作成する。
     * @param target
     * @return
     */
    @JCMethod
    public static ObjMonitoring createInstance(Object target) {
        return new ObjMonitoring(target);
    }
}
