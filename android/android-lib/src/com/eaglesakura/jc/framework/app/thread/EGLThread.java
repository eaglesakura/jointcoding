package com.eaglesakura.jc.framework.app.thread;

import static junit.framework.Assert.*;

import com.eaglesakura.jc.Jointable;
import com.eaglesakura.jc.egl.DeviceManager;
import com.eaglesakura.jc.framework.app.JointApplicationRenderer;
import com.eaglesakura.jc.jni.Pointer;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * EGL操作用のスレッドを作成する
 */
public class EGLThread extends Thread implements Jointable {

    /**
     * DeviceManagerのポインタを取得する
     */
    public static final int EGLTHREAD_POINTERKEY_DEVICEMANAGER = 1;

    /**
     * 主体となるレンダリングクラス
     */
    JointApplicationRenderer renderer;

    /**
     * レンダリングデバイス
     */
    DeviceManager device;

    /**
     * 操作用コールバック
     */
    Task task;

    /**
     * アプリコンテキスト
     */
    Pointer appContext;

    /**
     * 
     * @param renderer
     * @param device
     */
    public EGLThread(JointApplicationRenderer renderer, DeviceManager device, Pointer appContext, Task task) {
        this.renderer = renderer;
        this.device = device;
        this.task = task;
        this.appContext = appContext;
    }

    /**
     * レンダリングクラスを取得する
     * @return
     */
    public JointApplicationRenderer getRenderer() {
        return renderer;
    }

    /**
     * 操作スレッド専用デバイス
     * @return
     */
    public DeviceManager getThreadDevice() {
        return device;
    }

    @Override
    @JCMethod
    public void setNativePointer(int key, Pointer ptr) {
        assertTrue(false);
    }

    /**
     * 各種ポインタを取得する
     */
    @Override
    @JCMethod
    public Pointer getNativePointer(int key) {
        if (key == Jointable.KEY_MAINCONTEXT) {
            return appContext;
        } else if (key == EGLTHREAD_POINTERKEY_DEVICEMANAGER) {
            return device.getNativePointer(Jointable.KEY_MAINCONTEXT);
        }
        return null;
    }

    /**
     * スレッドの実行を行う
     * 実行後、デバイスは廃棄される。
     */
    @Override
    public void run() {
        task.run(this);
        // デバイスの廃棄
        device.dispose();

        // ネイティブの廃棄
        appContext.release();
        appContext = null;
    }

    /**
     * Native実行タスク
     */
    public interface Task {
        void run(EGLThread thread);
    }
}
