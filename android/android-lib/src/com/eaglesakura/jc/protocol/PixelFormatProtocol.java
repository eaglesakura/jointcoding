package com.eaglesakura.jc.protocol;

import android.graphics.PixelFormat;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;

/**
 * OpenGLのレンダリングを行うためのプロトコル
 * 
 *
 */
@JCClass(
         mode = Mode.Protocol,
         cppNamespace = "ndk")
public interface PixelFormatProtocol {
    @JCField
    public static final int ANDROID_PIXELFORMAT_RGB565 = PixelFormat.RGB_565;
    @JCField
    public static final int ANDROID_PIXELFORMAT_RGB888 = PixelFormat.RGB_888;
    @JCField
    public static final int ANDROID_PIXELFORMAT_RGBA8888 = PixelFormat.RGBA_8888;
}
