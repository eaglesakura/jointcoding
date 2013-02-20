package com.eaglesakura.jc.android.ui;

import java.util.ArrayList;
import java.util.List;

import android.view.MotionEvent;

import com.eaglesakura.jc.android.IJointCodingClass;
import com.eaglesakura.jc.protocol.TouchEventProtocol;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * タッチイベントをネイティブに伝えるための中間クラス。
 * このクラスのインスタンスをネイティブに渡して、ネイティブ側で処理させる。
 * 
 *
 */
@JCClass(
         cppNamespace = "ndk")
public class NativeTouchEvent implements TouchEventProtocol, IJointCodingClass {

    MotionEvent event;

    int eventType;

    /**
     * タッチX位置
     */
    float posX = 0;

    /**
     * タッチY位置
     */
    float posY = 0;

    /**
     * ポインタごとの一意のID
     */
    int id;

    public NativeTouchEvent(MotionEvent event, int index) {
        this.event = event;

        final int action = event.getAction() & 0xff;

        // タッチ座標を取り出す
        try {
            posX = event.getX(index);
            posY = event.getY(index);
        } catch (Exception e) {

        }

        // idを取り出す
        try {
            //            event.getPoi
            //            id = pointerIndex;
            //            id = index;
            id = event.getPointerId(index);
        } catch (Exception e) {

        }

        // イベントタイプを取り出す
        {
            switch (action) {
                case MotionEvent.ACTION_DOWN:
                case MotionEvent.ACTION_POINTER_DOWN:
                    eventType = EVENT_TYPE_BEGIN;
                    break;
                case MotionEvent.ACTION_MOVE:
                    eventType = EVENT_TYPE_MOVE;
                    break;
                case MotionEvent.ACTION_POINTER_UP:
                case MotionEvent.ACTION_UP:
                    eventType = EVENT_TYPE_END;
                    break;
                default:
                    //                    AndroidUtil.log("actin = " + action);
                    // ACTION_CANCELED
                    eventType = EVENT_TYPE_CANCEL;
                    break;
            }
        }

    }

    /**
     * タッチイベントYを取得する
     */
    @Override
    @JCMethod
    public float getEventPosX() {
        return posX;
    }

    /**
     * タッチイベントの位置を取得する
     */
    @Override
    @JCMethod
    public float getEventPosY() {
        return posY;
    }

    @Override
    @JCMethod
    public int getTouchID() {
        return id;
    }

    /**
     * イベントの種類を取得する。
     */
    @Override
    @JCMethod
    public int getEventType() {
        return eventType;
    }

    /**
     * 内容を文字列化する
     */
    @JCMethod
    @Override
    public String toString() {
        return "touch v(" + getEventPosX() + ", " + getEventPosY() + ") type( " + eventType + ") id( " + getTouchID()
                + ")";
    }

    private static boolean isSupportAction(int action) {
        switch (action & MotionEvent.ACTION_MASK) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_MOVE:
            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_DOWN:
            case MotionEvent.ACTION_POINTER_UP:
                return true;
        }
        return false;
    }

    /**
     * ネイティブイベントに変換する。
     * @param event
     * @return
     */
    public static List<NativeTouchEvent> toNativeEvents(MotionEvent event) {
        List<NativeTouchEvent> touchEvents = new ArrayList<NativeTouchEvent>();

        if (event.getAction() == MotionEvent.ACTION_MOVE) {
            for (int i = 0; i < event.getPointerCount(); ++i) {
                touchEvents.add(new NativeTouchEvent(event, i));
            }
        } else if (isSupportAction(event.getAction())) {
            touchEvents.add(new NativeTouchEvent(event, event.getActionIndex()));
        }

        return touchEvents;
    }
}
