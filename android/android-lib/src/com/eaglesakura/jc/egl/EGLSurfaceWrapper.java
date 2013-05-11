package com.eaglesakura.jc.egl;

import static javax.microedition.khronos.egl.EGL10.*;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLSurface;

import com.eaglesakura.jc.util.AndroidUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "ndk")
public class EGLSurfaceWrapper {

    /**
     * サーフェイス情報
     */
    EGLSurface eglSurface = null;

    /**
     * EGL情報
     */
    EGLWrapper egl = null;

    /**
     * 幅
     */
    int width = 0;

    /**
     * 高さ
     */
    int height = 0;

    EGLSurfaceWrapper(EGLWrapper egl, EGLSurface surface) {
        this.egl = egl;
        this.eglSurface = surface;

        onSurfaceResized();
    }

    /**
     * サーフェイスの回復を図る
     * @param eglSurface
     */
    void restore(EGLSurface eglSurface) {
        this.eglSurface = eglSurface;
        onSurfaceResized();
    }

    /**
     * サーフェイスを取得する
     * @return
     */
    public EGLSurface getSurface() {
        return eglSurface;
    }

    @JCMethod
    public int getWidth() {
        return width;
    }

    @JCMethod
    public int getHeight() {
        return height;
    }

    /**
     * サーフェイスリサイズ後に問い合わせる
     */
    public void onSurfaceResized() {
        // 幅と高さを問い合わせる
        width = egl.querySurface(eglSurface, EGL_WIDTH);
        height = egl.querySurface(eglSurface, EGL_HEIGHT);

        AndroidUtil.log(String.format("TextureView Surface Size from query = %d x %d", width, height));
    }

    /**
     * サーフェイスサイズを変更する
     * @param width
     * @param height
     */
    public void setSurfaceSize(int width, int height) {
        this.width = width;
        this.height = height;
    }

    /**
     * 解放処理を行う
     */
    @JCMethod
    public void dispose() {
        if (eglSurface == null) {
            return;
        }

        EGL10 egl = this.egl.getEGL();
        egl.eglDestroySurface(this.egl.getDisplay(), eglSurface);
        eglSurface = null;
    }

    /**
     * 有効な場合true
     * @return
     */
    public boolean valid() {
        return eglSurface != null;
    }
}
