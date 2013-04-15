package com.eaglesakura.jc.protocol;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         mode = Mode.Protocol,
         cppNamespace = "jc")
public interface KeyEventProtocol {
    /**
     * 押下イベント
     */
    @JCField
    public static final int EVENT_TYPE_DOWN = 0;

    /**
     * 離したイベント
     */
    @JCField
    public static final int EVENT_TYPE_UP = 1;

    /**
     * 不明なキーコード
     */
    @JCField
    public static final int KEYCODE_UNKNOWN = -1;

    /**
     * 上キー
     */
    @JCField
    public static final int KEYCODE_UP = 0;

    /**
     * 下キー
     */
    @JCField
    public static final int KEYCODE_DOWN = 1;

    /**
     * 左キー
     */
    @JCField
    public static final int KEYCODE_LEFT = 2;

    /**
     * 右キー
     */
    @JCField
    public static final int KEYCODE_RIGHT = 3;

    /**
     * 決定キー
     */
    @JCField
    public static final int KEYCODE_ENTER = 4;

    /**
     * 戻るキー
     */
    @JCField
    public static final int KEYCODE_BACK = 5;

    /**
     * イベントの種類を取得する
     * @return
     * @see #EVENT_TYPE_DOWN
     * @see #EVENT_TYPE_UP
     */
    @JCMethod
    public int getEventType();

    /**
     * 押されたキーのキーコードを取得する
     * @return
     */
    @JCMethod
    public int getKeyCode();
}
