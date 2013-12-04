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

    static final int QueryKey_Reserved = 0x00100000;

    /**
     * 現在の実行状態問い合わせ
     */
    @JCField
    public static final int PostKey_QueryApplicationState = QueryKey_Reserved + 0;

    /**
     * 初期化時に必要なサーフェイスの組み合わせを問い合わせる
     * result[0] = ピクセルフォーマット {@link com.eaglesakura.jcprotocol.PixelFormatProtocol}
     * result[1] = 0以外で深度バッファ有り
     * result[2] = 0以外でステンシルバッファ有り
     * result[3] = 0以外でTextureViewで利用
     */
    @JCField
    public static final int PostKey_RequestSurfaceSpecs = QueryKey_Reserved + 1;

    /**
     * 現在のサーフェイスサイズを書き込む
     */
    @JCField
    public static final int PostKey_SurfaceSize = QueryKey_Reserved + 2;

    /**
     * ステートの変更リクエストを送る
     */
    @JCField
    public static final int PostKey_StateRequest = QueryKey_Reserved + 3;

    /**
     * {@value #QueryKey_RequestSurfaceSpecs}が要求する長さ
     */
    @JCField
    public static final int QueryKey_RequestSurfaceSpecs_length = 4;

    /**
     * デフォルトで用意するタスク
     * メインループ用
     */
    @JCField
    public static final int SystemTask_Mainloop = 0x01000000;
}
