package com.eaglesakura.jc.android.app;

import java.util.UUID;

import android.content.Context;
import android.os.Looper;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * Native <--> SDKのコンテキストやり取りを行う。
 * アプリ実行中はインスタンスが継続して利用される。
 * 
 *
 */
@JCClass(
         cppNamespace = "ndk")
public class NativeContext {
    /**
     * コンテキスト本体
     */
    Context appContext = null;

    /**
     * class読み込み時に記録されるID.
     * 例えばapkがunloadされた場合はリセットされる。
     */
    static final String bootId = UUID.randomUUID().toString();

    private NativeContext(Context appContext) {
        this.appContext = appContext;
    }

    /**
     * アプリ制御用のContextを取得する
     * @return
     */
    @JCMethod
    public Context getAppContext() {
        return appContext;
    }

    /**
     * ネイティブ側の初期化を行う
     */
    @JCMethod(
              nativeMethod = true)
    native void nativeInitialize();

    /**
     * シングルトンインスタンス
     */
    private static NativeContext g_instance = null;

    /**
     * 起動IDを取得する。
     * @return
     */
    @JCMethod
    public static String getBootingId() {
        return bootId;
    }

    /**
     * UIスレッドで動作していたらtrueを返す
     * @return
     */
    @JCMethod
    public static boolean isUIThread() {
        return Thread.currentThread().equals(Looper.getMainLooper().getThread());
    }

    /**
     * 不要なメモリの整理を行う。
     */
    public static void gc() {
        System.gc();
        nativeGC();
    }

    /**
     * native側のGCを行う
     */
    @JCMethod(
              nativeMethod = true)
    static native void nativeGC();

    /**
     * 
     * @param context
     * @return
     */
    public static synchronized NativeContext getInstance(Context context) {
        if (g_instance == null) {
            g_instance = new NativeContext(context.getApplicationContext());
            g_instance.nativeInitialize();
        }
        return g_instance;
    }
}
