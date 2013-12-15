package com.eaglesakura.jcprotocol.framework;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;

@JCClass(
         mode = Mode.Protocol,
         cppNamespace = "jc")
public interface SurfaceSpecProtocol {

    /**
     * 初期化時のピクセルフォーマット
     * {@link com.eaglesakura.jcprotocol.PixelFormatProtocol}
     */
    @JCField
    public String KEY_PixelFormat = "PixelFormat";

    /**
     * 深度バッファを持つか持たないか
     * true/false
     */
    @JCField
    public String KEY_HasDepth = "depth";

    @JCField
    public String KEY_HasStencil = "stencil";

    /**
     * Android版でTextureViewを利用する
     */
    @JCField
    public String KEY_AndroidTextureView = "ATexureView";

    /**
     * Android版でSurfaceViewのZオーダーを指定する
     */
    @JCField
    public String KEY_AndroidSurfaceViewOnTop = "ASurfaceZ";
}
