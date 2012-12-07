package com.eaglesakura.jc.android.app;

import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "ndk")
public class AndroidUtil {
    /**
     * 単純にsleepさせる。
     * 
     * @param timems
     */
    public static void sleep(long timems) {
        try {
            Thread.sleep(timems);
        } catch (Exception e) {
        }
    }

    @JCMethod(
              nativeMethod = true)
    static native void _log(String message);

    public static void log(String message) {
        //        _log(message);
        Log.i("JC-LOG", message);
    }

    public static void log(Exception e) {
        //        _log(message);
        e.printStackTrace();
        Log.i("JC-LOG", e.getMessage());
    }

    /**
     * サーフェイスホルダからサーフェイス本体を取得する
     * @param holder
     * @return
     */
    @JCMethod
    public static Surface getSurface(SurfaceHolder holder) {
        return holder.getSurface();
    }
}
