package com.eaglesakura.jc.jni;

import android.os.Build;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * Nativeからの呼び出しサポートを行う
 */
@JCClass(
         cppNamespace = "ndk")
public class JniUtil {

    /**
     * APIレベルを取得する
     * @return
     */
    @JCMethod
    public static int getAPILevel() {
        return Build.VERSION.SDK_INT;
    }

    /**
     * スタックトレースを出力する
     * @param e
     */
    @JCMethod
    public static void printStackTrace(Exception e) {
        if (e != null) {
            e.printStackTrace();
        }
    }
}
