package com.eaglesakura.jc.framework.app;

import com.eaglesakura.jc.Jointable;
import com.eaglesakura.jc.egl.DeviceManager;
import com.eaglesakura.jc.egl.WindowDeviceManager;
import com.eaglesakura.jc.framework.app.thread.EGLThread;
import com.eaglesakura.jc.jni.Pointer;
import com.eaglesakura.jc.util.AndroidUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * TextureView1枚でアプリ管理を行うFragmentを構築する
 * iOSとの共通処理を記述するため、ある程度用途を限定する。
 * 
 * 現状では主にゲーム系のメインループ処理が必要なアプリとして考える。
 */
@JCClass(
         cppNamespace = "ndk")
public abstract class JointApplicationRenderer implements Jointable, WindowDeviceManager.SurfaceListener {
    /**
     * GPU管理クラス
     */
    WindowDeviceManager windowDevice = null;

    /**
     * 排他制御のためのロックオブジェクト
     */
    final Object lock = new Object();

    /**
     * アプリのメインコンテキスト
     * Native側で管理する
     */
    Pointer appContext = null;

    /**
     * フラグメント自体のステートを管理する
     */
    protected enum State {
        /**
         * 初期化中
         */
        Null,

        /**
         * 休止リクエスト中
         */
        Pausing,

        /**
         * 休止中
         */
        Paused,

        /**
         * 再開作業中
         */
        Resuming,

        /**
         * 復旧済み
         */
        Running,

        /**
         * サーフェイスのリサイズ中
         */
        SurfaceResizing,

        /**
         * 廃棄済み
         */
        Destroyed,

        /**
         * アプリを終了させた状態
         */
        Aborted,
    }

    /**
     * 初期化前
     */
    protected State state = State.Null;

    public JointApplicationRenderer() {
    }

    /**
     * フラグメント管理ステートを取得する
     * @return
     */
    public State getState() {
        return state;
    }

    @Override
    public void onEGLInitializeCompleted(WindowDeviceManager device) {
        synchronized (lock) {
            if (!validNative()) {
                // 初期化完了したデバイスを保持する
                this.windowDevice = device;

                createNativeContext(device);
                // 初期化されていなければエラーである
                if (appContext == null) {
                    throw new IllegalStateException("appContext == null");
                }

                // 初期化を行わせる
                onNativeInitialize();

                // メインループを開始する
                startMainLoop();
            }
        }
    }

    @Override
    public void onEGLSurfaceSizeChanged(WindowDeviceManager device, int width, int height) {
        state = State.SurfaceResizing;
        synchronized (lock) {
            if (validNative()) {
                AndroidUtil.log(String.format("Surface Size Changed(%d x %d)", width, height));
                onNativeSurfaceResized(width, height);
            }
        }
        state = State.Running;
    }

    /**
     * EGLの廃棄に伴い、コンテキストも廃棄を行う
     */
    @Override
    public void onEGLDestroyBegin(WindowDeviceManager device) {
        waitNativeDestroyed();
    }

    /**
     * アプリの休止を行う
     */
    public void onAppPause() {
        state = State.Pausing;

        while (state != State.Paused) {
            AndroidUtil.sleep(0, 100);
        }
    }

    /**
     * アプリのレジュームを行う
     */
    public void onAppResume() {
        state = State.Resuming;
    }

    /**
     * アプリの廃棄を行う
     */
    public void onAppDestroy() {
        waitNativeDestroyed();
    }

    /**
     * Native Appの廃棄を行う
     */
    final synchronized void waitNativeDestroyed() {
        if (state == State.Aborted || state == State.Destroyed) {
            return;
        }

        state = State.Destroyed;
        while (state != State.Aborted) {
            AndroidUtil.sleep(0, 100);
        }

        // コンテキストを廃棄する
        if (appContext != null) {
            appContext.release();
            appContext = null;
        }
    }

    /**
     * Nativeのクラスが有効である場合true
     * @return
     */
    protected boolean validNative() {
        return appContext != null;
    }

    /**
     * 解放済みであったらtrue
     * @return
     */
    protected boolean isDestroyed() {
        if (state == State.Destroyed || state == State.Aborted) {
            return true;
        }

        return false;
    }

    /**
     * GL側の操作が可能であればtrue
     * @return
     */
    protected boolean validGLOperation() {
        if (state != State.Running) {
            // Running状態でない場合は何も出来ない
            return false;
        }

        // デバイスの正常チェック
        if (windowDevice != null && !windowDevice.valid()) {
            // デバイスが不正な状態にある
            return false;
        }

        // 状態が有効でない場合はfalse
        if (!valid()) {
            return false;
        }

        // その他の状況は問題ない
        return true;
    }

    /**
     * Rendererが有効であればtrueを返す
     * @return
     */
    public boolean valid() {
        if (!validNative()) {
            return false;
        }
        return true;
    }

    /**
     * ネイティブコンテキストを取り出す
     */
    @Override
    @JCMethod
    public Pointer getNativePointer(int key) {
        if (key == Jointable.KEY_MAINCONTEXT) {
            return appContext;
        }

        return null;
    }

    /**
     * ネイティブコンテキストを設定する
     */
    @Override
    @JCMethod
    public void setNativePointer(int key, Pointer ptr) {
        if (key == Jointable.KEY_MAINCONTEXT) {
            appContext = ptr;
        }
    }

    /**
     * デバイス管理クラスを取得する
     * @return
     */
    @JCMethod
    public final DeviceManager getDeviceManager() {
        return windowDevice;
    }

    /**
     * ネイティブ側のメインループ処理を行う
     */
    @JCMethod(
              nativeMethod = true)
    final native void onNativeMainLoop();

    /**
     * ネイティブ側の初期化を行わせる
     */
    @JCMethod(
              nativeMethod = true)
    final native void onNativeInitialize();

    /**
     * サーフェイスサイズが変更になった
     * @param newWidth
     * @param newHeight
     */
    @JCMethod(
              nativeMethod = true)
    final native void onNativeSurfaceResized(int newWidth, int newHeight);

    /**
     * Fragment休止を行う
     */
    @JCMethod(
              nativeMethod = true)
    final native void onNativePause();

    /**
     * Fragment復帰を行う
     */
    @JCMethod(
              nativeMethod = true)
    final native void onNativeResume();

    /**
     * Fragment廃棄を行う
     */
    @JCMethod(
              nativeMethod = true)
    final native void onNativeDestroy();

    /**
     * レンダリングサーフェイスの色情報をチェックする
     * @return
     */
    final native int getSurfaceColorSpecRequest();

    /**
     * 深度バッファをリクエストしているかをチェックする
     * @return
     */
    final native boolean isSurfaceDepthRequest();

    /**
     * ステンシルバッファをリクエストしているかをチェックする
     * @return
     */
    final native boolean isSurfaceStencilRequest();

    /**
     * メインスレッドの処理を行う
     */
    protected void onMainThread() {
        // 状態が有効ならメインループを実行する
        while (state != State.Destroyed) {
            int sleepTimeMS = 0;

            synchronized (lock) {
                if (validGLOperation()) {
                    // 操作可能な状態であればメインループ処理を行う
                    onNativeMainLoop();
                } else {
                    switch (state) {
                        case Paused:
                            // 休止中なら長めにSleepして構わない
                            sleepTimeMS = 10;
                        default:
                            // その他は同期待ちだから短めのsleepをかける
                            sleepTimeMS = 1;
                            break;
                    }

                    if (state == State.Resuming && windowDevice.valid()) {
                        // レジュームリクエストが送られているので、レジューム処理を行わせる
                        onNativeResume();
                        sleepTimeMS = 0;
                        state = State.Running;
                    } else if (state == State.Pausing) {
                        // 休止リクエストが送られているので、休止処理を行う
                        onNativePause();
                        state = State.Paused;
                    }
                }
            }

            // 休眠命令があるなら適当な時間休眠する
            if (sleepTimeMS > 0) {
                // その他の状況であれば休止する
                AndroidUtil.sleep(sleepTimeMS);
            }
        }

        // 廃棄を行う
        synchronized (lock) {
            onNativeDestroy();
        }
        AndroidUtil.log("abort Rendering");
        state = State.Aborted;
    }

    /**
     * メインループを開始する
     */
    protected void startMainLoop() {
        Thread thread = new Thread() {
            @Override
            public void run() {
                onMainThread();
            }
        };
        thread.setName("jc-render");
        thread.start();

    }

    /**
     * アプリコンテキストポインタを取得する
     * @return
     */
    protected Pointer retainAppContextPointer() {
        synchronized (lock) {
            Pointer result = appContext;
            if (result != null) {
                result.retain();
            }
            return result;
        }
    }

    /**
     * 操作スレッドを生成する
     * @param slave スレイブデバイス（ロード用デバイス）を生成する場合はtrue、それ以外はレンダリングデバイスを利用する
     * @param threadName
     * @return
     */
    protected EGLThread newThread(boolean slave, String threadName, EGLThread.Task task) {
        DeviceManager device;
        if (slave) {
            device = windowDevice.createSlaveDevice();
        } else {
            device = windowDevice;
        }

        EGLThread thread = new EGLThread(this, device, retainAppContextPointer(), task);
        thread.setName(threadName);
        return thread;
    }

    /**
     * Native Contextを作成する。
     */
    protected abstract void createNativeContext(DeviceManager deviceManager);

}
