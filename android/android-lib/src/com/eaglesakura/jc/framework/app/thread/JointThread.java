package com.eaglesakura.jc.framework.app.thread;

import com.eaglesakura.jc.framework.app.JointApplicationRenderer;
import com.eaglesakura.jc.jni.Pointer;

/**
 * 操作用のスレッドを作成する
 */
public class JointThread extends Thread {
    /**
     * 主体となるレンダリングクラス
     */
    JointApplicationRenderer renderer;

    /**
     * 操作用コールバック
     */
    Task task;

    /**
     * アプリコンテキスト
     * ネイティブ側のクラスがdeleteされるのを防ぐ
     */
    Pointer appContext;

    /**
     * 
     * @param renderer
     * @param device
     */
    public JointThread(JointApplicationRenderer renderer, Pointer appContext, Task task) {
        this.renderer = renderer;
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
     * スレッドの実行を行う
     * 実行後、デバイスは廃棄される。
     */
    @Override
    public void run() {
        task.run(this);

        // ネイティブの廃棄
        // 参照カウンタを廃棄する
        appContext.release();
        appContext = null;
    }

    /**
     * Native実行タスク
     */
    public interface Task {
        void run(JointThread thread);
    }
}
