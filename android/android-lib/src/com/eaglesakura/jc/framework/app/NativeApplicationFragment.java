package com.eaglesakura.jc.framework.app;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

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

    Pointer appContext = null;

    public NativeApplicationFragment() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        surface = new GLNativeTextureView(getActivity());
        surface.initialize(0xFFFFFFFF, this);
        return surface;
    }

    @Override
    public void onResume() {
        super.onResume();

        synchronized (lock) {
            if (validNative()) {
                onNativeResume();
            }
        }
    }

    @Override
    public void onPause() {
        synchronized (lock) {
            if (validNative()) {
                onNativePause();
            }
        }
        super.onPause();
    }

    @Override
    public void onDestroy() {
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
            }
        }
    }

    @Override
    public void onEGLSurfaceSizeChanged(GLNativeTextureView view, int width, int height) {
        synchronized (lock) {
            if (validNative()) {
                onNativeSurfaceResized(width, height);
            }
        }
    }

    @Override
    public void onEGLSurfaceDestroyBegin(GLNativeTextureView view) {
        synchronized (lock) {
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
}
