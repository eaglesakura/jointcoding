package com.eaglesakura.jc.android.egl;

import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.view.TextureView;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;

/**
 * OpenGL ESデバイスを抽象化したクラス
 * 
 * TextureViewの動作に対応する
 * SurfaceView動作にも対応できるように組んでいるが、現状実装は行わない方針
 */
@JCClass(
         cppNamespace = "ndk")
public class DeviceManager implements TextureView.SurfaceTextureListener {
    final Object lock = new Object();

    /**
     * callbackで受け取ったSDKのウィンドウシステム
     */
    Object native_window = null;

    /**
     * EGL本体
     */
    EGLWrapper egl = null;

    /**
     * EGLContext
     */
    EGLContextWrapper eglContext = null;

    /**
     * EGLSurface
     */
    EGLSurfaceWrapper eglSurface = null;

    /**
     * 
     */
    EGLConfigChooser configChooser;

    SurfaceListener listener;

    public DeviceManager(EGLConfigChooser configChooser) {
        this.configChooser = configChooser;
    }

    /**
     * リスナー登録を行う
     * @param listener
     */
    public void setListener(SurfaceListener listener) {
        this.listener = listener;
    }

    private void onCreateSurface() {
        synchronized (lock) {
            // EGL初期化を行う
            if (egl != null) {
                egl = new EGLWrapper();
                egl.initialize(configChooser);

                eglContext = egl.createContext();
                eglSurface = egl.createSurface(native_window);

                listener.onEGLInitializeCompleted(this);
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
            if (eglSurface != null) {
                eglSurface.onSurfaceResized();
                eglSurface.setSurfaceSize(width, height);

                listener.onEGLSurfaceSizeChanged(this, width, height);
            }
        }
    }

    /**
     * サーフェイスの廃棄が行われたら呼び出される
     * あくまでウィンドウサーフェイスの廃棄のみが目的であり、Contextを解放したい場合は別途 {@link #dispose()} を呼び出す必要がある
     */
    private void onSurfaceDestroyed() {
        synchronized (lock) {
            if (eglSurface != null) {
                listener.onEGLSurfaceDestroyBegin(this);
                eglSurface.dispose();
                listener.onEGLSurfaceDestroyCompleted(this);
            }
        }
    }

    /**
     * デバイスの完全な解放処理を行う
     */
    public void dispose() {
        synchronized (lock) {
            if (eglSurface != null) {
                eglSurface.dispose();
                eglSurface = null;
            }

            if (eglContext != null) {
                eglContext.dispose();
                eglContext = null;
            }

            if (egl != null) {
                egl.dispose();
                egl = null;
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
    }

    /**
     * for TextureView.
     */
    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        onSurfaceDestroyed();

        // auto release SurfaceTexture
        return true;
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
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
        // not impl
    }

    public EGLWrapper getEGLWrapper() {
        return egl;
    }

    public EGLContextWrapper getEGLContextWrapper() {
        return eglContext;
    }

    public EGLSurfaceWrapper getEGLSurfaceWrapper() {
        return eglSurface;
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
        void onEGLInitializeCompleted(DeviceManager device);

        /**
         * EGLのサイズが変わった
         * @param view
         * @param width
         * @param height
         */
        void onEGLSurfaceSizeChanged(DeviceManager device, int width, int height);

        /**
         * EGLをサスペンドした。
         * @param view
         */
        void onEGLSurfaceDestroyBegin(DeviceManager device);

        /**
         * EGLサスペンドが完了した
         * @param view
         */
        void onEGLSurfaceDestroyCompleted(DeviceManager device);
    }
}
