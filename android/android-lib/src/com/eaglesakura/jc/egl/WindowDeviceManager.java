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
     * callbackで受け取ったSDKのウィンドウシステム
     */
    Object native_window = null;

    /**
     * 
     */
    EGLConfigChooser configChooser;

    /**
     * リスナー
     * 常に!=nullである必要がある
     */
    SurfaceListener listener;

    public WindowDeviceManager(EGLConfigChooser configChooser, SurfaceListener listener) {
        this.configChooser = configChooser;
        this.listener = listener;
    }

    /**
     * サーフェイスの生成が完了したら呼び出される
     */
    private void onCreateSurface() {
        synchronized (lock) {
            if (egl == null) {
                // EGL初期化を行う
                egl = new EGLWrapper();
                egl.initialize(configChooser);

                eglContext = egl.createContext();
                eglSurface = egl.createSurface(native_window);

                // NDK側のデバイスを生成する
                egl.current(eglContext, eglSurface);
                {
                    createNative();
                }
                egl.current(null, null);

                if (ndkDevice == null) {
                    throw new RuntimeException("Native Device not initialized");
                }

                listener.onEGLInitializeCompleted(this);
            } else {
                // サーフェイスの回復を図る
                egl.restoreSurface(eglSurface, native_window);
            }
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

            listener.onEGLSurfaceSizeChanged(this, width, height);
        }
    }

    /**
     * サーフェイスの廃棄が行われたら呼び出される
     * あくまでウィンドウサーフェイスの廃棄のみが目的であり、Contextを解放したい場合は別途 {@link #dispose()} を呼び出す必要がある
     */
    void onSurfaceDestroyed() {
        synchronized (lock) {
            if (eglSurface != null) {
                eglSurface.dispose();
            }
        }
    }

    /**
     * for TextureView.
     */
    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        this.native_window = surface;
        onCreateSurface();
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
        this.native_window = holder;
        onCreateSurface();
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
            listener.onEGLDestroyBegin(this);

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
     * ウィンドウサーフェイスのライフサイクル処理に合わせたコールバックを行う
     */
    public interface SurfaceListener {

        /**
         * EGL初期化が完了した。
         * 呼び出し後、onEGLResume()がよばれる。
         * @param view
         */
        void onEGLInitializeCompleted(WindowDeviceManager device);

        /**
         * EGLのサイズが変わった
         * @param view
         * @param width
         * @param height
         */
        void onEGLSurfaceSizeChanged(WindowDeviceManager device, int width, int height);

        /**
         * EGL解放を開始する
         * @param device
         */
        void onEGLDestroyBegin(WindowDeviceManager device);
    }
}
