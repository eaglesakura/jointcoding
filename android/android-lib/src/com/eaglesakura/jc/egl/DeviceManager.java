package com.eaglesakura.jc.egl;

import android.opengl.GLSurfaceView.EGLConfigChooser;

import com.eaglesakura.jc.Jointable;
import com.eaglesakura.jc.jni.Pointer;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "ndk")
public class DeviceManager implements Jointable {

    protected final Object lock = new Object();

    /**
     * 
     */
    protected EGLWrapper egl = null;

    /**
     * 
     */
    protected EGLContextWrapper eglContext = null;

    /**
     * 
     */
    protected EGLSurfaceWrapper eglSurface = null;

    /**
     * NDK側のレンダリングデバイス
     */
    Pointer ndkDevice;

    /**
     * 新規にデバイスを生成する
     * @param chooser
     */
    protected DeviceManager(EGLConfigChooser configChooser) {

        // EGLを生成する
        {
            egl = new EGLWrapper();
            egl.initialize(configChooser);
        }

        // コンテキスト/仮サーフェイスを生成する
        {
            eglContext = egl.createContext();
            eglSurface = egl.createPBufferSurface(1, 1);
        }

        // NDK側のデバイスを生成する
        egl.current(eglContext, eglSurface);
        {
            createNative();
        }
        egl.current(null, null);

        // デバイスの完全性チェック
        if (ndkDevice == null) {
            throw new RuntimeException("Native Device not initialized");
        }
    }

    /**
     * マスターデバイスを元に、スレイブデバイスを生成する
     * @param master
     */
    protected DeviceManager(DeviceManager master) {
        this.egl = master.getEGLWrapper().createSlaveEGL();
        this.eglContext = egl.createSharedContext(master.getEGLContextWrapper());
        this.eglSurface = egl.createPBufferSurface(1, 1);

        // NDK側のデバイスを生成する
        egl.current(eglContext, eglSurface);
        {
            createNative();
        }
        egl.current(null, null);

        // デバイスの完全性チェック
        if (ndkDevice == null) {
            throw new RuntimeException("Native Device not initialized");
        }
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

    /**
     * デバイスが有効な場合true
     * @return
     */
    public boolean valid() {
        if (eglSurface != null && eglSurface.valid()) {
            return true;
        } else {
            return false;
        }
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
     * デバイスの完全な解放処理を行う
     * 既にNDK側でDeviceを得ている場合は解放しても問題ない
     */
    public void dispose() {
        synchronized (lock) {
            if (ndkDevice != null) {
                ndkDevice.release();
            }
        }
    }

    /**
     * ネイティブ側のDeviceクラスを生成する
     */
    @JCMethod(
              nativeMethod = true)
    final native void createNative();

    /**
     * デバイスの操作を開始する
     */
    public void beginOperation() {
        synchronized (lock) {
            onNativeBeginOperation();
        }
    }

    /**
     * デバイス操作を終了する
     */
    public void endOperation() {
        synchronized (lock) {
            onNativeEndOperation();
        }
    }

    @JCMethod(
              nativeMethod = true)
    native void onNativeBeginOperation();

    @JCMethod(
              nativeMethod = true)
    native void onNativeEndOperation();
}