package com.eaglesakura.jc.android.egl.view;

import android.content.Context;
import android.util.AttributeSet;
import android.view.TextureView;
import android.view.View;

import com.eaglesakura.jc.android.egl.DefaultEGLConfigChooser;
import com.eaglesakura.jc.android.egl.DeviceManager;
import com.eaglesakura.jc.android.egl.WindowDeviceManager;
import com.eaglesakura.jc.android.egl.SurfaceColorSpec;

public class EGLTextureView extends TextureView implements RenderingSurface {

    final Object lock = new Object();

    WindowDeviceManager deviceManager = null;

    public EGLTextureView(Context context) {
        super(context);
    }

    public EGLTextureView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public EGLTextureView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    @Override
    public DeviceManager getDevice() {
        return deviceManager;
    }

    @Override
    public View getView() {
        return this;
    }

    @Override
    public void dispose() {
        synchronized (lock) {
            deviceManager.dispose();
            deviceManager = null;
        }
    }

    /**
     * 初期化を行う.
     * {@link android.app.Activity#setContentView(android.view.View)} されるより前に行わなければならない
     * @param colorSpec
     * @param hasDepth
     * @param hasStencil
     * @param listener
     */
    public void initialize(SurfaceColorSpec colorSpec, boolean hasDepth, boolean hasStencil,
            WindowDeviceManager.SurfaceListener listener) {
        synchronized (lock) {
            if (deviceManager != null) {
                throw new RuntimeException("EGLTextureView is initialized");
            }

            DefaultEGLConfigChooser chooser = new DefaultEGLConfigChooser();
            chooser.setColorSpec(colorSpec);
            chooser.setDepthEnable(hasDepth);
            chooser.setStencilEnable(hasStencil);

            deviceManager = new WindowDeviceManager(chooser, listener);

            // サーフェイスリスナを更新する
            setSurfaceTextureListener(deviceManager);
        }
    }
}
