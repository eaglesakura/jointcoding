package com.eaglesakura.jc.android.resource.jni;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * SDK <--> NDK接続のためのインターフェースを定義する。
 */
@JCClass(
         cppNamespace = "ndk")
public interface Jointable {
    /**
     * ポインタを設定する
     * @param key
     * @param ptr
     */
    @JCMethod
    public void setNativePointer(int key, Pointer ptr);

    /**
     * ポインタを取り出す
     * @param key
     * @return
     */
    @JCMethod
    public Pointer getNativePointer(int key);
}
