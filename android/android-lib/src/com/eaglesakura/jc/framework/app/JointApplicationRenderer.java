package com.eaglesakura.jc.framework.app;

import com.eaglesakura.jc.android.app.AndroidUtil;
import com.eaglesakura.jc.android.egl.DeviceManager;
import com.eaglesakura.jc.android.resource.jni.Jointable;
import com.eaglesakura.jc.android.resource.jni.Pointer;
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
public abstract class JointApplicationRenderer implements Jointable, DeviceManager.SurfaceListener {

    /**
     * GPU管理クラス
     */
    DeviceManager deviceManager = null;

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
         * 休止中
         */
        Paused,

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
         * レンダリングループも抜けた
         */
        RenderingFinished,
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
    public void onEGLInitializeCompleted(DeviceManager device) {
        synchronized (lock) {
            if (!validNative()) {
                // 初期化完了したデバイスを保持する
                this.deviceManager = device;

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
    public void onEGLSurfaceDestroyBegin(DeviceManager device) {
    }

    @Override
    public void onEGLSurfaceSizeChanged(DeviceManager device, int width, int height) {
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
    public void onEGLDestroyBegin(DeviceManager device) {
        synchronized (lock) {
            if (validNative()) {
                onNativeDestroy();
                appContext.dispose();
                appContext = null;
            }

            // デバイスを保持する意味はなくなった
            this.deviceManager = null;
        }
    }

    /**
     * アプリの休止を行う
     */
    public void onAppPause() {
        state = State.Paused;
        synchronized (lock) {
            if (validNative()) {
                onNativePause();
            }
        }
    }

    /**
     * アプリのレジュームを行う
     */
    public void onAppResume() {
        synchronized (lock) {
            if (validNative()) {
                onNativeResume();
            }
        }
        state = State.Running;
    }

    /**
     * アプリの廃棄を行う
     */
    public void onAppDestroy() {
        state = State.Destroyed;
        // レンダリングの終了待ちを行う
        while (state != State.RenderingFinished) {
            AndroidUtil.sleep(1);
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
     * GL側の操作が可能であればtrue
     * @return
     */
    protected boolean validGLOperation() {
        if (state != State.Running) {
            // Running状態でない場合は何も出来ない
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
    public DeviceManager getDeviceManager() {
        return deviceManager;
    }

    /**
     * ネイティブ側のメインループ処理を行う
     */
    @JCMethod(
              nativeMethod = true)
    protected native void onNativeMainLoop();

    /**
     * サーフェイスサイズが変更になった
     * @param newWidth
     * @param newHeight
     */
    @JCMethod(
              nativeMethod = true)
    protected native void onNativeInitialize();

    /**
     * サーフェイスサイズが変更になった
     * @param newWidth
     * @param newHeight
     */
    @JCMethod(
              nativeMethod = true)
    protected native void onNativeSurfaceResized(int newWidth, int newHeight);

    /**
     * Fragment休止を行う
     */
    @JCMethod(
              nativeMethod = true)
    protected native void onNativePause();

    /**
     * Fragment復帰を行う
     */
    @JCMethod(
              nativeMethod = true)
    protected native void onNativeResume();

    /**
     * Fragment廃棄を行う
     */
    @JCMethod(
              nativeMethod = true)
    protected native void onNativeDestroy();

    /**
     * Native Contextを作成する。
     */
    protected abstract void createNativeContext(DeviceManager deviceManager);

    /**
     * メインループ実行クラスを作成する
     * @return
     */
    protected Runnable newMainLoopRunner() {
        return new Runnable() {
            @Override
            public void run() {
                // 状態が有効ならメインループを実行する
                while (state != State.Destroyed) {
                    boolean sleep = false;
                    synchronized (lock) {
                        if (validGLOperation()) {
                            // 操作可能な状態であればメインループ処理を行う
                            onNativeMainLoop();
                            sleep = true;
                        } else {
                            sleep = true;
                        }
                    }

                    // 休眠命令があるなら適当な時間休眠する
                    if (sleep) {
                        // その他の状況であれば休止する
                        AndroidUtil.sleep(10);
                    }
                }

                AndroidUtil.log("abort Rendering");
                state = State.RenderingFinished;
            }
        };
    }

    /**
     * メインループを開始する
     */
    protected void startMainLoop() {
        Runnable runner = newMainLoopRunner();

        Thread thread = new Thread(runner);
        thread.setName("jc-render");
        thread.start();
    }
}
