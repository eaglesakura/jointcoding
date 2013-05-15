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

    /**
     * ウィンドウ描画が可能なサーフェイスならばtrue
     */
    boolean windowSurface = false;

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

        AndroidUtil.log(String.format("EGLSurface Size from query = %d x %d", width, height));
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
     * ウィンドウ描画が可能なサーフェイスならばtrueを返す
     * @return
     */
    public boolean isWindowSurface() {
        return windowSurface;
    }

    /**
     * 有効な場合true
     * @return
     */
    public boolean valid() {
        return eglSurface != null;
    }

    /**
     * 中身を入れ替える
     * WindowSurfaceと仮バインド用のPBufferSurfaceを入れ替えるために利用する
     */
    public void swap(EGLSurfaceWrapper surface) {
        synchronized (this) {
            synchronized (surface) {

                // EGLSurface入れ替え
                {
                    final EGLSurface tempSurface = this.eglSurface;
                    this.eglSurface = surface.eglSurface;
                    surface.eglSurface = tempSurface;
                }

                // ウィンドウサイズ入れ替え
                {
                    final int tempWidth = this.width;
                    this.width = surface.width;
                    surface.width = tempWidth;
                }
                {
                    final int tempHeight = this.height;
                    this.height = surface.height;
                    surface.height = tempHeight;
                }

                // ウィンドウサーフェイスフラグ入れ替え
                {
                    final boolean tempWindowSurface = this.windowSurface;
                    this.windowSurface = surface.windowSurface;
                    surface.windowSurface = tempWindowSurface;
                }
            }
        }
    }
}
