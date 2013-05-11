package com.google.android.gles_jni;

import java.util.Locale;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

import android.util.Log;

import com.eaglesakura.jc.jni.Pointer;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass
public class EGLSupport {
    static final String TAG = EGLSupport.class.getSimpleName();

    static final String CLASS_EGLCONTEXT_IMPL = "com.google.android.gles_jni.EGLContextImpl";

    static final String CLASS_EGLDISPLAY_IMPL = "com.google.android.gles_jni.EGLDisplayImpl";

    static final String CLASS_EGLCONFIG_IMPL = "com.google.android.gles_jni.EGLConfigImpl";

    /**
     * EGLConfigサポートを作成する
     * @param config
     * @return
     */
    private static Object newSupportClass(String eglClassName, int state) {
        eglClassName = eglClassName.replaceAll("\\.", "/");
        return newSupportClassNative(eglClassName, state);
    }

    /**
     * Native経由で取り出す
     * @param eglClassName
     * @param state
     * @return
     */
    @JCMethod(
              nativeMethod = true)
    static native Object newSupportClassNative(String eglClassName, int state);

    /**
     * Native経由でIntFieldを取り出す
     * @param obj
     * @param name
     * @return
     */
    @JCMethod(
              nativeMethod = true)
    static native int getIntFieldNative(Class<?> clazz, Object obj, String name);

    /**
     * eglCreateWindowSurfaceを行う
     * @param display
     * @param config
     * @param device
     * @return
     */
    @JCMethod
    public static int eglCreateWindowSurfaceSupport(int display, int config, Object native_window) {
        if (native_window == null) {
            Log.e(TAG, "native_window is null !!");
        }
        EGL10 egl = (EGL10) EGLContext.getEGL();

        EGLDisplay eglDisplay = (EGLDisplay) newSupportClass(CLASS_EGLDISPLAY_IMPL, display);
        EGLConfig eglConfig = (EGLConfig) newSupportClass(CLASS_EGLCONFIG_IMPL, config);

        Log.d(TAG, String.format(Locale.getDefault(), "eglDisplay(%s), eglConfig(%s)", "" + eglDisplay, "" + eglConfig));

        EGLSurface surface = egl.eglCreateWindowSurface(eglDisplay, eglConfig, native_window, null);
        if (surface == EGL10.EGL_NO_SURFACE) {
            Log.e(TAG, "EGL_NO_SURFACE...");
            return Pointer.NULL;
        }

        // surfaceからmEGLSurfaceを取り出す
        return getIntFieldNative(surface.getClass(), surface, "mEGLSurface");
    }
}
