package com.eaglesakura.jc.android.app;

import java.util.UUID;

import android.content.Context;
import android.os.Looper;
import android.util.DisplayMetrics;
import android.widget.Toast;

import com.eaglesakura.jc.android.thread.UIHandler;
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
     * デバッグ用のトーストを表示する
     * @param message
     */
    @JCMethod
    public static void showToast(final String message, final boolean longTime) {
        if (!isUIThread()) {
            UIHandler.postUI(new Runnable() {
                @Override
                public void run() {
                    showToast(message, longTime);
                }
            });
            return;
        }
        Toast.makeText(g_instance.appContext, message, longTime ? Toast.LENGTH_LONG : Toast.LENGTH_SHORT).show();
    }

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

    public static Context getApplicationContext() {
        return g_instance.getAppContext();
    }

    /**
     * 整数リソースを取得する
     * @param integer_id
     * @return
     */
    @JCMethod
    public static int getInteger(int integer_id) {
        return getApplicationContext().getResources().getInteger(integer_id);
    }

    /**
     * 色リソースを取得する
     * @param color_id
     * @return
     */
    @JCMethod
    public static int getColorRGBA(int color_id) {
        int argb = getApplicationContext().getResources().getColor(color_id);
        return (argb << 8) | ((argb >> 24) & 0xFF);
    }

    /**
     * dimenを取得する
     * @param id
     * @return
     */
    @JCMethod
    public static float getDimension(int id) {
        return getApplicationContext().getResources().getDimension(id);
    }

    /**
     * 文字列を取得する
     * @param id
     * @return
     */
    @JCMethod
    public static String getString(int id) {
        return getApplicationContext().getString(id);
    }

}
