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

    /**
     * 
     */
    @JCField
    public static final int ORIENTATION_ROTATE_0 = 0;

    /**
     * 
     */
    @JCField
    public static final int ORIENTATION_ROTATE_90 = 1;

    /**
     * 
     */
    @JCField
    public static final int ORIENTATION_ROTATE_180 = 2;

    /**
     * 
     */
    @JCField
    public static final int ORIENTATION_ROTATE_270 = 3;

    /**
     * オートフォーカス動いていない
     */
    @JCField
    public static final int FOCUSMODE_NONE = 0;

    /**
     * オートフォーカス起動中
     */
    @JCField
    public static final int FOCUSMODE_PROCESSING = 1;

    /**
     * オートフォーカス完了
     */
    @JCField
    public static final int FOCUSMODE_COMPLETED = 2;

    /**
     * オートフォーカス終了
     */
    @JCField
    public static final int FOCUSMODE_FAILED = 3;
}
