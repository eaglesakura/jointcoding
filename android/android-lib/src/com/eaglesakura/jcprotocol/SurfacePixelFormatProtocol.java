package com.eaglesakura.jcprotocol;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;

/**
 * レンダリングサーフェイスのピクセルフォーマットを定義する
 */
@JCClass(
         mode = Mode.Protocol,
         cppNamespace = "jc")
public interface SurfacePixelFormatProtocol {

    @JCField
    public static final int RGB8 = 0;

    @JCField
    public static final int RGBA8 = 1;
}
