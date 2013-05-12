package com.eaglesakura.jc.egl;

import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.view.SurfaceHolder;
import android.view.TextureView;

/**
 * OpenGL ESデバイスを抽象化したクラス
 * 
 * TextureViewの動作に対応する
 * SurfaceView動作にも対応できるように組んでいるが、現状実装は行わない方針
 */
public class WindowDeviceManager extends DeviceManager implements TextureView.SurfaceTextureListener,
        SurfaceHolder.Callback {

    /**
     * リスナー
     * 常に!=nullである必要がある
     */
    SurfaceListener listener;

    /**
     * サーフェイス廃棄済みフラグ
     */
    boolean destroyed = false;

    public WindowDeviceManager(EGLConfigChooser configChooser, SurfaceListener listener) {
        super(configChooser);
        this.listener = listener;

    }

    /**
     * サーフェイスの生成が完了したら呼び出される
     */
    private void onCreateSurface(Object native_window) {
        synchronized (lock) {
            // サーフェイスの回復を図る
            egl.restoreWindowSurface(eglSurface, native_window);

            if (destroyed) {
                // レストア完了メッセージを送る
            } else {
                // 初期化完了
                listener.onSurfaceInitializeCompleted(this);
            }

            destroyed = false;
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
            eglSurface.onSurfaceResized();
            eglSurface.setSurfaceSize(width, height);

            listener.onSurfaceSurfaceSizeChanged(this, width, height);
        }
    }

    /**
     * サーフェイスの廃棄が行われたら呼び出される
     * あくまでウィンドウサーフェイスの廃棄のみが目的であり、Contextを解放したい場合は別途 {@link #dispose()} を呼び出す必要がある
     */
    void onSurfaceDestroyed() {
        synchronized (lock) {
            if (eglSurface != null) {
                egl.restorePBufferSurface(eglSurface, 1, 1);
            }

            destroyed = true;
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

    @Override
    public void dispose() {
        synchronized (lock) {
            // リスナを呼び出す
            listener.onSurfaceDestroyBegin(this);

            // 解放処理を行う
            super.dispose();
        }
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

    /**
     * ロード用のスレイブデバイスを生成する
     * @return
     */
    public DeviceManager createSlaveDevice() {
        return new DeviceManager(this);
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
