package com.eaglesakura.jc.android.thread;

import android.os.Handler;
import android.os.Looper;

/**
 * UIスレッド専用のハンドラ
 * 
 *
 */
public class UIHandler extends Handler {

    public UIHandler() {
        super(Looper.getMainLooper());
    }

    private static UIHandler instance = null;

    /**
     * 唯一のインスタンスを取得する。
     * @return UIHandlerインスタンス
     */
    public static UIHandler getInstance() {
        if (instance == null) {
            instance = new UIHandler();
        }
        return instance;
    }

    /**
     * UIスレッドで実行を行わせる。
     * @param runnable
     */
    public static void postUI(Runnable runnable) {
        getInstance().post(runnable);
    }

    /**
     * 指定したディレイをかけてPOSTする
     * @param runnable
     * @param delay
     */
    public static void postDelayedUI(Runnable runnable, long delay) {
        getInstance().postDelayed(runnable, delay);
    }

    public static boolean isUIThread() {
        return Thread.currentThread().equals(Looper.getMainLooper().getThread());
    }

    /**
     * Handlerに関連付けられていたThreadで動作している場合はtrueを返す。
     * @param handler
     * @return
     */
    public static boolean isHandlerThread(Handler handler) {
        return Thread.currentThread().equals(handler.getLooper().getThread());
    }

}
