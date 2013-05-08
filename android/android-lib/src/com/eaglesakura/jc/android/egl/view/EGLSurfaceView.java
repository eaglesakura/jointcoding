package com.eaglesakura.jc.android.egl.view;

import android.content.Context;
import android.graphics.PixelFormat;
import android.util.AttributeSet;
import android.view.SurfaceView;
import android.view.View;

import com.eaglesakura.jc.android.egl.DefaultEGLConfigChooser;
import com.eaglesakura.jc.android.egl.DeviceManager;
import com.eaglesakura.jc.android.egl.SurfaceColorSpec;

public class EGLSurfaceView extends SurfaceView implements RenderingSurface {

    /**
     * ロックオブジェクト
     */
    final Object lock = new Object();

    /**
     * 
     */
    DeviceManager deviceManager = null;

    public EGLSurfaceView(Context context) {
        super(context);
    }

    public EGLSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public EGLSurfaceView(Context context, AttributeSet attrs, int defStyle) {
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
            DeviceManager.SurfaceListener listener) {
        synchronized (lock) {
            if (deviceManager != null) {
                throw new RuntimeException("EGLTextureView is initialized");
            }

            DefaultEGLConfigChooser chooser = new DefaultEGLConfigChooser();
            chooser.setColorSpec(colorSpec);
            chooser.setDepthEnable(hasDepth);
            chooser.setStencilEnable(hasStencil);

            deviceManager = new DeviceManager(chooser, listener);

            // サーフェイスリスナを更新する
            if (colorSpec.getAlphaSize() > 0) {
                getHolder().setFormat(PixelFormat.RGBA_8888);
            } else {
                getHolder().setFormat(PixelFormat.RGB_888);
            }
            getHolder().addCallback(deviceManager);
        }
    }
}
