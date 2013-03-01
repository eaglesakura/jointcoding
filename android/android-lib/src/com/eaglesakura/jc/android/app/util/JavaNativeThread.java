package com.eaglesakura.jc.android.app.util;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * NDK側から生成し、コールバックする。
 * boost::threadの実装ではjniの呼び出しが行えない場合があるため、Javaから呼びつける方式を取る。
 * 
 *
 */
@JCClass(
         cppNamespace = "ndk")
public class JavaNativeThread extends Thread {
    int nativeThread;

    public JavaNativeThread(int nativeThread) {
        this.nativeThread = nativeThread;
    }

    /**
     * スレッドの終了待ちを行う
     */
    @JCMethod
    public void threadJoin() {
        try {
            this.join();
        } catch (Exception e) {

        }
    }

    /**
     * スレッド名を設定する
     * @param name
     */
    @JCMethod
    public void setThreadName(String name) {
        setName(name);
    }

    /**
     * スレッドを開始する
     */
    @JCMethod
    @Override
    public synchronized void start() {
        super.start();
    }

    /**
     * 実処理を行う
     */
    @JCMethod
    @Override
    public void run() {
        onBackground(nativeThread);
    }

    /**
     * 裏スレッドで実行する
     * @param nativeThread
     */
    @JCMethod(
              nativeMethod = true)
    public static native void onBackground(int nativeThread);

    /**
     * インスタンスを作成する
     * @param nativeThread
     * @return
     */
    @JCMethod
    public static JavaNativeThread createInstance(int nativeThread) {
        return new JavaNativeThread(nativeThread);
    }
}
