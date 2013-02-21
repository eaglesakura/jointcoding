package com.eaglesakura.jc.android.view;

import android.content.Context;
import android.graphics.SurfaceTexture;
import android.util.AttributeSet;
import android.view.TextureView;

import com.eaglesakura.jc.android.IJointCodingClass;
import com.eaglesakura.jc.android.resource.jni.Pointer;
import com.eaglesakura.jc.android.resource.jni.Pointer.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**

 * OpenGL ES 2.0に最適化したTextureViewを構築する。
 * 
 * サーフェイス作成後、{@link #initialize(int)}を呼び出してEGLの初期化準備を明示的に行わなければならい。
 * また、初期化準備はUIスレッドから行うことを前提としている。
 * setContentView()される前にinitialize()を呼び出すことを想定している。
 * 
 * サーフェイスの復旧等は自動的に行われるが、廃棄する場合は必ず{@link #destroy()}を呼び出し、Native側の資源を開放しなければならない。
 * 
 */
@JCClass(
         cppNamespace = "ndk")
public class GLNativeTextureView extends TextureView implements TextureView.SurfaceTextureListener, IJointCodingClass {

    static final String TAG = GLNativeTextureView.class.getSimpleName();
    /**
     * デバイスポインタ
     */
    private Pointer nativeContext = null;

    /**
     * 初期化済みならtrue
     */
    private boolean initialized = false;

    /**
     * GL初期化済みならtrue
     */
    private boolean glInitialized = false;

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
     * ステンシルバッファを有効にする
     */
    public static final int EGL_STENCILBUFFER_ENABLE = 0x1 << 1;

    /**
     * カラーバッファのビット深度をデフォルトにする
     */
    @JCField
    public static final int EGL_COLORBUFFER_BITS_DEFAULT = 0x00000000;

    public GLNativeTextureView(Context context) {
        super(context);
    }

    public GLNativeTextureView(Context context, AttributeSet attr) {
        super(context, attr);
    }

    public GLNativeTextureView(Context context, AttributeSet attr, int style) {
        super(context, attr, style);
    }

    /**
     * NativeContextが初期化済みであることを保証する
     */
    private void assertNativeInitialized() {
        if (!initialized) {
            throw new RuntimeException(TAG + " not initialized!!");
        }

        // 廃棄済みかをチェックする
        if (getNativeContextPointer() == Pointer.NULL) {
            throw new RuntimeException(TAG + " is destroyed...");
        }
    }

    /**
     * Viewの初期化を行う
     * @param egl_flags
     * @param callback
     */
    public void initialize(int egl_flags, GLES2Callback callback) {
        if (initialized) {
            throw new RuntimeException(TAG + " Initialized!!");
        }
        if (callback == null) {
            throw new RuntimeException(TAG + " callback is null...");
        }

        int context = onCreateNativeContext(EGL_DEPTHBUFFER_ENABLE | EGL_STENCILBUFFER_ENABLE);
        if (context == Pointer.NULL) {
            throw new NullPointerException("native context is null...");
        }
        nativeContext = new Pointer(context, Mode.SharedObject);

        this.es2callback = callback;

        // 自身をリスナにする
        setSurfaceTextureListener(this);

        // 終了したので、初期化済みフラグを立てる
        initialized = true;
    }

    /**
     * TextureViewが有効になった
     */
    @Override
    public void onSurfaceTextureAvailable(SurfaceTexture surface, int width, int height) {
        assertNativeInitialized();
        if (!glInitialized) {
            onNativeGLInitialize(surface, width, height);
            glInitialized = true;
            es2callback.onEGLInitializeCompleted(this);
        }
        onNativeGLSurfaceSizeChanged(surface, width, height);
        es2callback.onEGLSurfaceSizeChanged(this, width, height);
    }

    /**
     * TextureViewが破壊された
     */
    @Override
    public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
        try {
            assertNativeInitialized();
            es2callback.onEGLSurfaceDestroyBegin(this);
            {
                onNativeGLDestroyed(surface);
            }
            es2callback.onEGLSurfaceDestroyCompleted(this);
        } catch (Exception e) {
        }
        return true;
    }

    /**
     * TextureViewサイズが更新された
     */
    @Override
    public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {
        onNativeGLSurfaceSizeChanged(surface, width, height);
        es2callback.onEGLSurfaceSizeChanged(this, width, height);
    }

    /**
     * TextureViewが描画された
     */
    @Override
    public void onSurfaceTextureUpdated(SurfaceTexture surface) {
    }

    /**
     * デバイス用のsp<>ポインタを取得する。
     * @return
     */
    @JCMethod
    protected final int getNativeContextPointer() {
        if (nativeContext == null) {
            return Pointer.NULL;
        }
        return nativeContext.getNativePointer();
    }

    /**
     * EGL初期化を行う
     * @param egl_flags
     * @return
     */
    @JCMethod(
              nativeMethod = true)
    native int onCreateNativeContext(int egl_flags);

    /**
     * Nativeの削除を行う
     */
    @JCMethod(
              nativeMethod = true)
    native void onDestroyNative();

    /**
     * SurfaceTextureを初期化する
     * @param surface
     * @param width
     * @param height
     */
    @JCMethod(
              nativeMethod = true)
    native void onNativeGLInitialize(SurfaceTexture surface, int width, int height);

    /**
     * SurfaceTextureを復旧する
     * @param surface
     * @param width
     * @param height
     */
    @JCMethod(
              nativeMethod = true)
    native void onNativeGLResume(SurfaceTexture surface, int width, int height);

    /**
     * SurfaceTextureのサイズ変更に対応する
     * @param surface
     * @param width
     * @param height
     */
    @JCMethod(
              nativeMethod = true)
    native void onNativeGLSurfaceSizeChanged(SurfaceTexture surface, int width, int height);

    /**
     * GLを休止する
     * @param surface
     */
    @JCMethod(
              nativeMethod = true)
    native void onNativeGLDestroyed(SurfaceTexture surface);

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
        void onEGLInitializeCompleted(GLNativeTextureView view);

        /**
         * EGLのサイズが変わった
         * @param view
         * @param width
         * @param height
         */
        void onEGLSurfaceSizeChanged(GLNativeTextureView view, int width, int height);

        /**
         * EGLをサスペンドした。
         * @param view
         */
        void onEGLSurfaceDestroyBegin(GLNativeTextureView view);

        /**
         * EGLサスペンドが完了した
         * @param view
         */
        void onEGLSurfaceDestroyCompleted(GLNativeTextureView view);
    }
}
