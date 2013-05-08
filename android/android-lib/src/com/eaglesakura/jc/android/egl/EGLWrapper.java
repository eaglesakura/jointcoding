package com.eaglesakura.jc.android.egl;

import static javax.microedition.khronos.egl.EGL10.*;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

import android.graphics.SurfaceTexture;
import android.opengl.GLSurfaceView.EGLConfigChooser;
import android.view.SurfaceHolder;

import com.eaglesakura.jc.android.app.AndroidUtil;
import com.eaglesakura.jc.android.app.NativeContext;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * EGL操作のラッピングを行う
 */
@JCClass(
         cppNamespace = "ndk")
public class EGLWrapper {
    /**
     * ロックオブジェクト
     */
    final private Object lock = new Object();

    /**
     * EGLオブジェクト
     */
    EGL10 egl = null;

    /**
     * レンダリング用ディスプレイ
     */
    EGLDisplay eglDisplay = null;

    /**
     * config情報
     */
    EGLConfig eglConfig = null;

    /**
     * システムがデフォルトで使用しているEGLDisplayオブジェクト
     */
    EGLDisplay systemDisplay = null;

    /**
     * システムがデフォルトで使用しているEGLSurface(Read)
     */
    EGLSurface systemReadSurface = null;

    /**
     * システムがデフォルトで使用しているEGLSurface(Draw)
     */
    EGLSurface systemDrawSurface = null;

    /**
     * システムがデフォルトで使用しているコンテキスト
     */
    EGLContext systemContext = null;

    public EGLWrapper() {
    }

    /**
     * 初期化を行う
     */
    public void initialize(final EGLConfigChooser chooser) {
        synchronized (lock) {
            if (egl != null) {
                throw new RuntimeException("initialized");
            }

            egl = (EGL10) EGLContext.getEGL();
            // ディスプレイ作成
            {
                eglDisplay = egl.eglGetDisplay(EGL_DEFAULT_DISPLAY);
                if (eglDisplay == EGL_NO_DISPLAY) {
                    throw new RuntimeException("EGL_NO_DISPLAY");
                }

                if (!egl.eglInitialize(eglDisplay, new int[2])) {
                    throw new RuntimeException("eglInitialize");
                }
            }
            // コンフィグ取得
            {
                stashContext();
                eglConfig = chooser.chooseConfig(egl, systemDisplay != EGL_NO_DISPLAY ? systemDisplay : eglDisplay);
                if (eglConfig == null) {
                    throw new RuntimeException("chooseConfig");
                }
            }
        }
    }

    /**
     * レンダリングコンテキストを生成する
     * @return
     */
    public EGLContextWrapper createContext() {
        EGLContext eglContext = egl.eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT,
        // attributes
                new int[] {
                        0x3098 /* EGL_CONTEXT_CLIENT_VERSION */, 2, EGL_NONE
                });

        if (eglContext == EGL10.EGL_NO_CONTEXT) {
            throw new RuntimeException("eglCreateContext");
        }

        return new EGLContextWrapper(this, eglContext);
    }

    /**
     * レンダリングサーフェイスを生成する
     * @param native_window
     * @return
     */
    public EGLSurfaceWrapper createSurface(Object native_window) {
        if (!(native_window instanceof SurfaceHolder) && !(native_window instanceof SurfaceTexture)) {
            throw new IllegalArgumentException("surface error");
        }

        // レンダリング用サーフェイスを再度生成
        EGLSurface eglSurface = egl.eglCreateWindowSurface(eglDisplay, eglConfig, native_window, new int[] {
            // attributes
                EGL_NONE
            });
        if (eglSurface == EGL10.EGL_NO_SURFACE) {
            throw new RuntimeException("eglCreateWindowSurface");
        }

        return new EGLSurfaceWrapper(this, eglSurface);
    }

    /**
     * コンフィグ情報を取得する
     * @return
     */
    public EGLConfig getConfig() {
        return eglConfig;
    }

    /**
     * サーフェイス内容を問い合わせる
     * @param surface
     * @param attribute
     * @return
     */
    public int querySurface(EGLSurface surface, int attribute) {
        int[] temp = new int[1];
        egl.eglQuerySurface(eglDisplay, surface, attribute, temp);
        return temp[0];
    }

    public EGL10 getEGL() {
        return egl;
    }

    public EGLDisplay getDisplay() {
        return eglDisplay;
    }

    /**
     * 解放処理を行う
     */
    @JCMethod
    public void dispose() {
        synchronized (lock) {
            if (egl == null) {
                return;
            }

            if (eglDisplay != null) {
                egl.eglTerminate(eglDisplay);
                eglDisplay = null;
            }

            egl = null;
        }
    }

    /**
     * システムコンテキストを保存する
     */
    private void stashContext() {
        try {
            // システムのデフォルトオブジェクトを取り出す
            systemDisplay = egl.eglGetCurrentDisplay();
            systemReadSurface = egl.eglGetCurrentSurface(EGL_READ);
            systemDrawSurface = egl.eglGetCurrentSurface(EGL_DRAW);
            systemContext = egl.eglGetCurrentContext();

            if (systemDisplay == null) {
                systemDisplay = EGL_NO_DISPLAY;
            }

            if (systemReadSurface == null) {
                systemReadSurface = EGL_NO_SURFACE;
            }

            if (systemDrawSurface == null) {
                systemDrawSurface = EGL_NO_SURFACE;
            }

            if (systemContext == null) {
                systemContext = EGL_NO_CONTEXT;
            }
        } catch (Exception e) {
            systemDisplay = EGL_NO_DISPLAY;
            systemReadSurface = EGL_NO_SURFACE;
            systemDrawSurface = EGL_NO_SURFACE;
            systemContext = EGL_NO_CONTEXT;
        }
    }

    /**
     * カレントコンテキストを設定する
     * @param context
     * @param surface
     */
    @JCMethod
    public boolean current(EGLContextWrapper context, EGLSurfaceWrapper surface) {
        synchronized (lock) {
            boolean result = false;
            if (context != null && surface != null) {
                if (NativeContext.isUIThread()) {
                    stashContext();
                }

                EGLContext eglContext = context.getContext();
                EGLSurface eglSurface = surface.getSurface();

                result = egl.eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
            } else if (context == null && surface == null) {
                EGLContext eglContext = EGL_NO_CONTEXT;
                EGLSurface eglReadSurface = EGL_NO_SURFACE;
                EGLSurface eglDrawSurface = EGL_NO_SURFACE;
                EGLDisplay eglDisplay = this.eglDisplay;

                // UIスレッドならば書き戻す
                if (NativeContext.isUIThread()) {
                    eglDisplay = systemDisplay;
                    eglReadSurface = systemReadSurface;
                    eglDrawSurface = systemDrawSurface;
                    eglContext = systemContext;
                }
                result = egl.eglMakeCurrent(eglDisplay, eglDrawSurface, eglReadSurface, eglContext);
            }

            return result;
        }
    }

    /**
     * フロントバッファ転送を行う
     * @param surface
     * @return
     */
    @JCMethod
    public boolean postFrontBuffer(EGLSurfaceWrapper surface) {
        synchronized (lock) {
            final EGLSurface targetSurface = surface.getSurface();
            final EGLSurface currentSurface = egl.eglGetCurrentSurface(EGL_DRAW);
            if (!currentSurface.equals(targetSurface)) {
                AndroidUtil.log("postFrontBuffer(targetSurface != currentSurface)");
                return false;
            }
            boolean result = egl.eglSwapBuffers(eglDisplay, targetSurface);
            return result;
        }
    }
}
