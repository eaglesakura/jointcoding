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
     * レジューム
     * request only
     */
    @JCField
    public static final int State_Resume = 4;

    static final int QueryKey_Reserved = 0x00100000;

    /**
     * 現在の実行状態問い合わせ
     */
    @JCField
    public static final int QueryKey_ApplicationState = QueryKey_Reserved + 0;

    /**
     * 現在のサーフェイスサイズを書き込む
     */
    @JCField
    public static final int PostKey_SurfaceSize = QueryKey_Reserved + 1;

    /**
     * ステートの変更リクエストを送る
     */
    @JCField
    public static final int PostKey_StateRequest = QueryKey_Reserved + 2;

}
