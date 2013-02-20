package com.eaglesakura.jc.android.app.util;

import android.os.Build;

import com.eaglesakura.jc.android.IJointCodingClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "ndk")
public class NativeAppUtil implements IJointCodingClass {

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
