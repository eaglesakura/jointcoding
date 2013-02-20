package com.eaglesakura.jc.android.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import com.eaglesakura.jc.android.IJointCodingClass;
import com.eaglesakura.jc.android.app.NativeContext;
import com.eaglesakura.jc.android.resource.jni.Pointer;
import com.eaglesakura.jc.android.resource.jni.Pointer.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * OpenGL ES 2.0に最適化したSurfaceViewを構築する。
 * 
 * サーフェイス作成後、{@link #initialize(int)}を呼び出してEGLの初期化準備を明示的に行わなければならい。
 * また、初期化準備はUIスレッドから行うことを前提としている。
 * setContentView()される前にinitialize()を呼び出すことを想定している。
 * 
 * サーフェイスの復旧等は自動的に行われるが、廃棄する場合は必ず{@link #destroy()}を呼び出し、Native側の資源を開放しなければならない。
 * 
 *
 */
@JCClass(
         cppNamespace = "ndk")
@Deprecated
public final class GLNativeSurfaceView extends SurfaceView implements IJointCodingClass {
    /**
     * デバイスポインタ
     */
    private Pointer nativeContext = null;

    /**
     * 初期化済みならtrue
     */
    private boolean initialized = false;

    /**
     * 初回レジューム時のみtrue
     */
    private boolean firstResume = true;

    /**
     * コールバック処理
     */
    private GLES2Callback es2callback = null;

    /**
     * 深度バッファを有効にする
     */
    @JCField
    public static final int EGL_DEPTHBUFFER_ENABLE = 0x1 << 0;

    /**
     * カラーバッファのビット深度をデフォルトにする
     */
    @JCField
    public static final int EGL_COLORBUFFER_BITS_DEFAULT = 0x00000000;

    public GLNativeSurfaceView(Context context) {
        super(context);
    }

    public GLNativeSurfaceView(Context context, AttributeSet attr) {
        super(context, attr);
    }

    public GLNativeSurfaceView(Context context, AttributeSet attr, int style) {
        super(context, attr, style);
    }

    /**
     * 初期化を行う。
     * @param egl_flags
     */
    public void initialize(int egl_flags, GLES2Callback callback) {
        if (initialized) {
            return;
        }

        if (!NativeContext.isUIThread()) {
            throw new RuntimeException("is not uithread!!");
        }

        int jc_sp_nativeContext = onInitializeNative(egl_flags);
        if (jc_sp_nativeContext == Pointer.NULL) {
            throw new RuntimeException("Device is null...");
        }
        nativeContext = new Pointer(jc_sp_nativeContext, Mode.SharedObject);

        this.es2callback = callback;

        // コールバック追加
        getHolder().addCallback(surfaceCallback);

        // 終了したので、初期化済みフラグを立てる
        initialized = true;
    }

    /**
     * native側の初期化を行う
     * @param egl_flags
     * @return jc_sp<ndk::gl::GLNativeSurfaceViewContext>のポインタ値
     */
    @JCMethod(
              nativeMethod = true)
    native int onInitializeNative(int egl_flags);

    /**
     * 解放処理を行う。
     */
    @JCMethod(
              nativeMethod = true)
    native void onDestroyNative();

    private GLNativeSurfaceView view() {
        return this;
    }

    private SurfaceHolder.Callback surfaceCallback = new SurfaceHolder.Callback() {
        @Override
        public void surfaceCreated(SurfaceHolder holder) {
            if (getNativeContextPointer() != Pointer.NULL) {
                onSurfaceCreated(holder, holder.getSurface());
            }
        }

        @Override
        public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
            if (getNativeContextPointer() != Pointer.NULL) {

                onSurfaceChanged(holder, holder.getSurface(), format, width, height);
                if (es2callback != null) {
                    if (firstResume) {
                        // Viewの初期化が完了したことを伝える。
                        es2callback.onEGLInitializeCompleted(view());
                        firstResume = false;
                    }

                    es2callback.onEGLResumeCompleted(view());
                }

            }
        }

        @Override
        public void surfaceDestroyed(SurfaceHolder holder) {
            if (getNativeContextPointer() != Pointer.NULL) {
                if (es2callback != null) {
                    es2callback.onEGLPauseBegin(view());
                }
                onSurfaceDestroyed(holder, holder.getSurface());
                if (es2callback != null) {
                    es2callback.onEGLPauseCompleted(view());
                }
            }
        }
    };

    @JCMethod(
              nativeMethod = true)
    final native void onSurfaceCreated(SurfaceHolder holder, Surface surface);

    @JCMethod(
              nativeMethod = true)
    final native void onSurfaceChanged(SurfaceHolder holder, Surface surface, int format, int width, int height);

    @JCMethod(
              nativeMethod = true)
    final native void onSurfaceDestroyed(SurfaceHolder holder, Surface surface);

    /**
     * デバイス用のsp<>ポインタを取得する。
     * @return
     */
    @JCMethod
    final int getNativeContextPointer() {
        if (nativeContext == null) {
            return Pointer.NULL;
        }
        return nativeContext.getNativePointer();
    }

    @JCMethod
    static final int getNativeContextPointer(GLNativeSurfaceView view) {
        if (view == null) {
            return Pointer.NULL;
        }

        if (view.nativeContext == null) {
            return Pointer.NULL;
        }

        return view.nativeContext.getNativePointer();
    }

    /**
     * Viewの解放を行う。
     * {@link android.app.Fragment#onDestroyView()}にて呼び出すこと。
     */
    public void destroy() {
        if (nativeContext != null) {
            onDestroyNative();
            nativeContext.dispose();
            nativeContext = null;
        }
    }

    /**
     * GLES2処理のコールバックを取得する。
     * 
     *
     */
    public interface GLES2Callback {
        /**
         * EGL初期化が完了した。
         * 呼び出し後、onEGLResume()がよばれる。
         * @param view
         */
        void onEGLInitializeCompleted(GLNativeSurfaceView view);

        /**
         * EGL復帰が完了した。
         * @param view
         */
        void onEGLResumeCompleted(GLNativeSurfaceView view);

        /**
         * EGLをサスペンドした。
         * @param view
         */
        void onEGLPauseBegin(GLNativeSurfaceView view);

        /**
         * EGLサスペンドが完了した
         * @param view
         */
        void onEGLPauseCompleted(GLNativeSurfaceView view);
    }
}
