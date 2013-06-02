package com.eaglesakura.jc.egl;

import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.view.SurfaceHolder;
import android.view.TextureView;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * OpenGL ESデバイスを抽象化したクラス
 * 
 * TextureViewの動作に対応する
 * SurfaceView動作にも対応できるように組んでいるが、現状実装は行わない方針
 */
@JCClass(
         cppNamespace = "ndk")
public class WindowDeviceManager extends DeviceManager implements TextureView.SurfaceTextureListener,
        SurfaceHolder.Callback {

    /**
     * リスナー
     * 常に!=nullである必要がある
     */
    SurfaceListener listener;

    /**
     * ウィンドウサーフェイスとPBufferサーフェイスの入れ替えを行うため、一時的に保持しておく
     * 
     * ウィンドウサーフェイスが利用不可能なタイミングでは、PBufferサーフェイスを利用させる。
     */
    EGLSurfaceWrapper restoreSurface;

    public WindowDeviceManager(EGLConfigChooser configChooser, SurfaceListener listener) {
        super(configChooser);
        this.listener = listener;

    }

    /**
     * サーフェイスの生成が完了したら呼び出される
     */
    private void onCreateSurface(Object native_window) {
        synchronized (lock) {
            beginOperation();

            // レストアフラグを保持しておく
            boolean restore = (restoreSurface != null);

            if (restoreSurface == null) {
                // ウィンドウサーフェイスの生成を行う
                restoreSurface = egl.createWindowSurface(native_window);
            } else {
                // ウィンドウサーフェイスの復旧を行う
                egl.restoreWindowSurface(restoreSurface, native_window);

            }

            // native側で参照の変更を行いたくないため、SDK側で
            // サーフェイスの中身を入れ替える
            eglSurface.swap(restoreSurface);

            if (!restore) {
                // 初期化完了
                listener.onSurfaceInitializeCompleted(this);
            } else {
                // レストア完了メッセージを送る
                listener.onSurfaceRestored(this);
            }

            endOperation();
        }
    }

    /**
     * サーフェイスのリサイズ処理が行われたら呼び出される
     * ウィンドウ廃棄を伴う場合はdestroyを経由するため、あくまでリサイズ処理として扱う
     * @param width
     * @param height
     */
    private void onResizedSurface(int width, int height) {
        synchronized (lock) {
            beginOperation();
            eglSurface.onSurfaceResized();
            eglSurface.setSurfaceSize(width, height);

            listener.onSurfaceSurfaceSizeChanged(this, width, height);
            endOperation();
        }
    }

    /**
     * サーフェイスの廃棄が行われたら呼び出される
     * あくまでウィンドウサーフェイスの廃棄のみが目的であり、Contextを解放したい場合は別途 {@link #dispose()} を呼び出す必要がある
     */
    void onSurfaceDestroyed() {
        synchronized (lock) {
            if (restoreSurface != null) {
                beginOperation();

                listener.onSurfaceDestroyBegin(this);

                // 古いウィンドウサーフェイスは廃棄する
                eglSurface.dispose();

                // サーフェイスの中身をPBufferに入れ替える
                eglSurface.swap(restoreSurface);

                endOperation();
            }

        }
    }

    /**
     * for TextureView.
     */
    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        onCreateSurface(surface);
        onResizedSurface(width, height);
    }

    /**
     * for TextureView.
     */
    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        onResizedSurface(width, height);
    }

    /**
     * for TextureView.
     */
    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        // auto release SurfaceTexture
        return true;
    }

    /**
     * for TextureView.
     */
    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        // not impl
    }

    /**
     * サーフェイスが生成/再生成された
     * 
     * for SurfaceView
     * @param holder
     */
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        onCreateSurface(holder);
    }

    /**
     * サーフェイスサイズが更新された
     * 
     * for SurfaceView
     * @param holder
     * @param format
     * @param width
     * @param height
     */
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        onResizedSurface(width, height);
    }

    /**
     * サーフェイスが廃棄された
     * 
     * for SurfaceView
     * @param holder
     */
    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        onSurfaceDestroyed();
    }

    @JCMethod
    @Override
    public void dispose() {
        synchronized (lock) {
            // リスナを呼び出す
            listener.onSurfaceDestroyBegin(this);

            // 一時的に保持していたバッファを解放する
            if (restoreSurface != null) {
                restoreSurface.dispose();
                restoreSurface = null;
            }

            // 解放処理を行う
            super.dispose();
        }
    }

    /**
     * ロード用のスレイブデバイスを生成する
     * @return
     */
    @JCMethod
    public DeviceManager createSlaveDevice() {
        DeviceManager result;
        beginOperation();
        {
            result = new DeviceManager(this);
        }
        endOperation();

        return result;
    }

    /**
     * ウィンドウサーフェイスのライフサイクル処理に合わせたコールバックを行う
     */
    public interface SurfaceListener {
        /**
         * EGL初期化が完了した。
         * 呼び出し後、onEGLResume()がよばれる。
         * @param view
         */
        void onSurfaceInitializeCompleted(WindowDeviceManager device);

        /**
         * サーフェイスの復旧に成功した
         * @param device
         */
        void onSurfaceRestored(WindowDeviceManager device);

        /**
         * EGLのサイズが変わった
         * @param view
         * @param width
         * @param height
         */
        void onSurfaceSurfaceSizeChanged(WindowDeviceManager device, int width, int height);

        /**
         * EGL解放を開始する
         * @param device
         */
        void onSurfaceDestroyBegin(WindowDeviceManager device);
    }
}
