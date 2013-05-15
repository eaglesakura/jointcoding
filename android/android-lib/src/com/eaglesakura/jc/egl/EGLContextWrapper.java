package com.eaglesakura.jc.egl;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLContext;

import com.eaglesakura.jc.util.AndroidUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "ndk")
public class EGLContextWrapper {

    /**
     * レンダリングコンテキスト
     */
    EGLContext eglContext;

    /**
     * EGL本体
     */
    EGLWrapper egl;

    EGLContextWrapper(EGLWrapper egl, EGLContext eglContext) {
        this.egl = egl;
        this.eglContext = eglContext;
    }

    /**
     * レンダリングコンテキストを取得する
     * @return
     */
    public EGLContext getContext() {
        return eglContext;
    }

    /**
     * 解放処理を行う
     */
    @JCMethod
    public void dispose() {
        if (eglContext == null) {
            return;
        }

        AndroidUtil.log(String.format("EGLContextWrapper#dispose(%d)", hashCode()));
        EGL10 egl = this.egl.getEGL();
        egl.eglDestroyContext(this.egl.getDisplay(), eglContext);
        eglContext = null;
    }
}
