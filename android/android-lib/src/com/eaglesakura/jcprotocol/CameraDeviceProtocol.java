package com.eaglesakura.jcprotocol;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;

@JCClass(
         mode = Mode.Protocol,
         cppNamespace = "jc")
public interface CameraDeviceProtocol {
    /**
     * メインカメラ
     */
    @JCField
    public static final int TYPE_MAIN = 0;

    /**
     * フロントカメラ
     */
    @JCField
    public static final int TYPE_FRONT = 1;
}
