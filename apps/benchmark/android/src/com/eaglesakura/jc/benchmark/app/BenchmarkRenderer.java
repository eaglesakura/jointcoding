package com.eaglesakura.jc.benchmark.app;

import com.eaglesakura.jc.android.app.AndroidUtil;
import com.eaglesakura.jc.android.egl.DeviceManager;
import com.eaglesakura.jc.android.egl.EGLContextWrapper;
import com.eaglesakura.jc.android.egl.EGLSurfaceWrapper;
import com.eaglesakura.jc.android.egl.EGLWrapper;
import com.eaglesakura.jc.framework.app.JointApplicationRenderer;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "es")
public class BenchmarkRenderer extends JointApplicationRenderer {

    public BenchmarkRenderer() {
    }

    /**
     * テクスチャを読み込む
     */
    @JCMethod(
              nativeMethod = true)
    native void loadTexture(DeviceManager deviceManager);

    @Override
    protected void startMainLoop() {
        super.startMainLoop();

        startSubLoop();
    }

    protected void startSubLoop() {
        Thread thread = new Thread() {
            @Override
            public void run() {

                DeviceManager deviceManager = getDeviceManager();
                EGLWrapper egl = deviceManager.getEGLWrapper().createSlaveEGL();

                AndroidUtil.log("surface");
                EGLSurfaceWrapper surface = egl.createPBufferSurface(1, 1);

                AndroidUtil.log("context");
                EGLContextWrapper context = egl.createSharedContext(deviceManager.getEGLContextWrapper());

                loadTexture(new DeviceManager(egl, context, surface));
                //
                //                context.dispose();
                //                surface.dispose();
            }
        };
        thread.setName("jc-sub0");
        thread.start();
    }

    @JCMethod(
              nativeMethod = true)
    @Override
    protected native void createNativeContext(DeviceManager deviceManager);
}
