package com.eaglesakura.jc.framework.app.thread;

import com.eaglesakura.jc.egl.DeviceManager;
import com.eaglesakura.jc.framework.app.JointApplicationRenderer;

/**
 * EGL操作用のスレッドを作成する
 */
public class EGLThread extends Thread {
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
    Runnable task;

    /**
     * 
     * @param renderer
     * @param device
     */
    public EGLThread(JointApplicationRenderer renderer, DeviceManager device, Runnable task) {
        this.renderer = renderer;
        this.device = device;
        this.task = task;
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
    public void run() {
        task.run();
    }
}
