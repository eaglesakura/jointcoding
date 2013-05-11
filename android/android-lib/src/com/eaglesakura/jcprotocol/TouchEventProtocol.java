package com.eaglesakura.jcprotocol;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * タッチイベント用の動作を制御する。
 * 
 *
 */
@JCClass(
         mode = Mode.Protocol,
         cppNamespace = "jc")
public interface TouchEventProtocol {

    /**
     * タッチが開始された
     */
    @JCField
    public static final int EVENT_TYPE_BEGIN = 0;

    /**
     * ポイントが移動された
     */
    @JCField
    public static final int EVENT_TYPE_MOVE = 1;

    /**
     * ポイントが離された / キャンセルされた
     */
    @JCField
    public static final int EVENT_TYPE_END = 2;

    /**
     * ポイントがキャンセルされた（View -> 別Viewに移ってフォーカスが外れた等）
     */
    @JCField
    public static final int EVENT_TYPE_CANCEL = 3;

    /**
     * イベントの種類を取得する
     * @return
     * @see #EVENT_TYPE_BEGIN
     * @see #EVENT_TYPE_MOVE
     * @see #EVENT_TYPE_END
     * @see #EVENT_ID_CANCEL
     */
    @JCMethod
    public int getEventType();

    /**
     * タッチ位置Xを取得する
     * @return
     */
    @JCMethod
    public float getEventPosX();

    /**
     * タッチ位置Yを取得する
     * @return
     */
    @JCMethod
    public float getEventPosY();

    /**
     * タッチの指のIDを取得する
     * @return
     */
    @JCMethod
    public int getTouchID();
}
