package com.eaglesakura.jc.android.egl;

import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.view.TextureView;

import com.eaglesakura.jc.android.resource.jni.Jointable;
import com.eaglesakura.jc.android.resource.jni.Pointer;
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
public class DeviceManager implements TextureView.SurfaceTextureListener, Jointable {
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

    /**
     * リスナー
     * 常に!=nullである必要がある
     */
    SurfaceListener listener;

    /**
     * NDK側のレンダリングデバイス
     */
    Pointer ndkDevice;

    public DeviceManager(EGLConfigChooser configChooser, SurfaceListener listener) {
        this.configChooser = configChooser;
        this.listener = listener;
    }

    /**
     * サーフェイスの生成が完了したら呼び出される
     */
    private void onCreateSurface() {
        synchronized (lock) {
            // EGL初期化を行う
            if (egl == null) {
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
                listener.onEGLSurfaceDestroyBegin(this);
                eglSurface.dispose();
            }
        }
    }

    /**
     * デバイスの完全な解放処理を行う
     */
    public void dispose() {
        synchronized (lock) {
            // リスナを呼び出す
            listener.onEGLSurfaceDestroyBegin(this);
            listener.onEGLDestroyBegin(this);

            // ネイティブ側の廃棄フラグを追加する
            // この状態で既にデバイスロックは行えなくなる
            preDestroyNative();
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

            if (ndkDevice != null) {
                ndkDevice.dispose();
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
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        dispose();

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

    @JCMethod
    public EGLWrapper getEGLWrapper() {
        return egl;
    }

    @JCMethod
    public EGLContextWrapper getEGLContextWrapper() {
        return eglContext;
    }

    @JCMethod
    public EGLSurfaceWrapper getEGLSurfaceWrapper() {
        return eglSurface;
    }

    @Override
    @JCMethod
    public Pointer getNativePointer(int key) {
        return ndkDevice;
    }

    @Override
    @JCMethod
    public void setNativePointer(int key, Pointer ptr) {
        ndkDevice = ptr;
    }

    /**
     * ネイティブ側のDeviceクラスを生成する
     */
    @JCMethod(
              nativeMethod = true)
    native void createNative();

    /**
     * 解放の前処理を行う
     */
    @JCMethod(
              nativeMethod = true)
    native void preDestroyNative();

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
         * EGLをサスペンド開始する
         * @param view
         */
        void onEGLSurfaceDestroyBegin(DeviceManager device);

        /**
         * EGL解放を開始する
         * @param device
         */
        void onEGLDestroyBegin(DeviceManager device);
    }
}
