package com.eaglesakura.jc.framework.app;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import com.eaglesakura.jc.android.app.AndroidUtil;
import com.eaglesakura.jc.android.resource.jni.Jointable;
import com.eaglesakura.jc.android.resource.jni.Pointer;
import com.eaglesakura.jc.android.view.GLNativeTextureView;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * TextureView1枚でアプリ管理を行うFragmentを構築する
 * iOSとの共通処理を記述するため、ある程度用途を限定する。
 * 
 * 現状では主にゲーム用途として考える。
 */
@JCClass(
         cppNamespace = "ndk")
public abstract class NativeApplicationFragment extends Fragment implements Jointable,
        GLNativeTextureView.GLES2Callback {

    /**
     * 排他制御のためのロックオブジェクト
     */
    final Object lock = new Object();

    /**
     * レンダリングサーフェイス
     */
    GLNativeTextureView surface = null;

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
    }

    /**
     * 初期化前
     */
    protected State state = State.Null;

    /**
     * タッチ制御をNativeに伝えるクラス
     */
    private View.OnTouchListener surfaceTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            return true;
        }
    };

    public NativeApplicationFragment() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        surface = new GLNativeTextureView(getActivity());
        surface.initialize(0xFFFFFFFF, this);
        surface.setOnTouchListener(surfaceTouchListener);
        return surface;
    }

    /**
     * 復帰処理を行う
     */
    @Override
    public void onResume() {
        super.onResume();

        synchronized (lock) {
            if (validNative()) {
                onNativeResume();
            }
        }
        state = State.Running;
    }

    /**
     * 休止処理を行う
     */
    @Override
    public void onPause() {
        state = State.Paused;
        synchronized (lock) {
            if (validNative()) {
                onNativePause();
            }
        }
        super.onPause();
    }

    /**
     * 廃棄を行う
     */
    @Override
    public void onDestroy() {
        state = State.Destroyed;
        synchronized (lock) {
            if (surface != null) {
                surface.destroy();
                surface = null;
            }
        }

        super.onDestroy();
    }

    @Override
    public void onEGLInitializeCompleted(GLNativeTextureView view) {
        synchronized (lock) {
            if (!validNative()) {
                createNativeContext(view);
                // 初期化されていなければエラーである
                if (appContext == null) {
                    throw new IllegalStateException("appContext == null");
                }

                // メインループを開始する
                startMainLoop();
            }
        }
    }

    @Override
    public void onEGLSurfaceSizeChanged(GLNativeTextureView view, int width, int height) {
        state = State.SurfaceResizing;
        synchronized (lock) {
            if (validNative()) {
                AndroidUtil.log(String.format("Surface Size Changed(%d x %d)", width, height));
                onNativeSurfaceResized(width, height);
            }
        }
        state = State.Running;
    }

    @Override
    public void onEGLSurfaceDestroyBegin(GLNativeTextureView view) {
        synchronized (lock) {
            AndroidUtil.log("Destroy Surface");
            if (validNative()) {
                onNativeDestroy();
            }
        }
    }

    @Override
    public void onEGLSurfaceDestroyCompleted(GLNativeTextureView view) {
        synchronized (lock) {
            if (validNative()) {
                appContext.dispose();
                appContext = null;
            }
        }
    }

    /**
     * フラグメント管理ステートを取得する
     * @return
     */
    public State getState() {
        return state;
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
     * レンダリングサーフェイスを取得する
     * @return
     */
    @JCMethod
    public GLNativeTextureView getSurface() {
        return surface;
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
     * Fragmentが有効であればtrueを返す
     * @return
     */
    public boolean valid() {
        if (getActivity() == null) {
            return false;
        }

        if (!validNative()) {
            return false;
        }

        if (isDetached()) {
            return false;
        }

        if (!isAdded()) {
            return false;
        }

        return true;
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
    protected abstract void createNativeContext(GLNativeTextureView surface);

    /**
     * メインループ実行クラスを作成する
     * @return
     */
    protected Runnable newMainLoopRunner() {
        return new Runnable() {
            @Override
            public void run() {
                // 状態が有効ならメインループを実行する
                while (true) {
                    boolean sleep = false;
                    synchronized (lock) {
                        if (validGLOperation()) {
                            // 操作可能な状態であればメインループ処理を行う
                            onNativeMainLoop();
                        } else if (state == State.Destroyed) {
                            AndroidUtil.log("abort Activity");
                            return;
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
            }
        };
    }

    /**
     * メインループを開始する
     */
    protected void startMainLoop() {
        Runnable runner = newMainLoopRunner();
        (new Thread(runner)).start();
    }
}
