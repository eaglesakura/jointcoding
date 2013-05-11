package com.eaglesakura.jc.egl;

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

    public DeviceManager() {
    }

    public DeviceManager(EGLWrapper egl, EGLContextWrapper context, EGLSurfaceWrapper surface) {
        this.egl = egl;
        this.eglContext = context;
        this.eglSurface = surface;

        // NDK側のデバイスを生成する
        egl.current(eglContext, eglSurface);
        {
            createNative();
        }
        egl.current(null, null);

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
     */
    public void dispose() {
        synchronized (lock) {
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
     * 解放の前処理を行う
     */
    @JCMethod(
              nativeMethod = true)
    final native void preDestroyNative();

}