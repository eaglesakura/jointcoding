package com.eaglesakura.jcprotocol.framework;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;

/**
 * アプリの実行状態を定義する
 */

@JCClass(
         cppNamespace = "jc",
         mode = Mode.Protocol)
public interface JointApplicationProtocol {
    /**
     * 初期化中
     */
    @JCField
    public static final int State_Initializing = 0;

    /**
     * 実行中
     */
    @JCField
    public static final int State_Running = 1;

    /**
     * 実行休止中
     */
    @JCField
    public static final int State_Paused = 2;

    /**
     * 終了済み
     */
    @JCField
    public static final int State_Destroyed = 3;

    /**
     * 現在の実行状態問い合わせ
     */
    @JCField
    public static final int QueryKey_ApplicationState = 0x00010000;

    /**
     * 現在のサーフェイスサイズを書き込む
     */
    @JCField
    public static final int PostKey_SurfaceSize = 0x00010001;
}
