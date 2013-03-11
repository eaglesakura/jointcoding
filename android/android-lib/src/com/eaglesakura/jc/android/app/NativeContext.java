package com.eaglesakura.jc.android.app;

import java.util.UUID;

import android.content.Context;
import android.os.Looper;
import android.util.DisplayMetrics;

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

    DisplayMetrics displayMetrics = null;

    /**
     * class読み込み時に記録されるID.
     * 例えばapkがunloadされた場合はリセットされる。
     */
    static final String bootId = UUID.randomUUID().toString();

    private NativeContext(Context appContext) {
        this.appContext = appContext;
        this.displayMetrics = appContext.getResources().getDisplayMetrics();
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
     * ディスプレイ構成を取得する
     * @return
     */
    public DisplayMetrics getDisplayMetrics() {
        return displayMetrics;
    }

    /**
     * dp値をpixelに変換する
     * @param dp
     * @return
     */
    public int dp2pixel(final float dp) {
        return (int) (dp * displayMetrics.density + 0.5f);
    }

    /**
     * pixel値をdp値に変換する
     * @param pixel
     * @return
     */
    public float pixel2dp(final int pixel) {
        return (float) pixel / displayMetrics.density;
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
    @JCMethod
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
     * Native側がNDK_DEBUG=1でビルドされている場合、trueが返る。
     * それ以外はfalseになる。
     * @return
     */
    @JCMethod(
              nativeMethod = true)
    public static native boolean isNativeDebuggable();

    /**
     * Native側がLOCAL_CPPFLAGS += OUTPUT_LOGされている場合、trueが返る。
     * @return
     */
    @JCMethod(
              nativeMethod = true)
    public static native boolean isNativeLogOutput();

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
