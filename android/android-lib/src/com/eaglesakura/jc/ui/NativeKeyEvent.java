package com.eaglesakura.jc.ui;

import java.util.HashMap;
import java.util.Map;

import android.annotation.SuppressLint;
import android.view.KeyEvent;

import com.eaglesakura.jcprotocol.KeyEventProtocol;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "ndk")
public class NativeKeyEvent implements KeyEventProtocol {

    /**
     * key:Androidキーコード
     * value:JCキーコード
     */
    @SuppressLint("UseSparseArrays")
    private static final Map<Integer, Integer> codeMaps = new HashMap<Integer, Integer>();

    static {
        codeMaps.put(KeyEvent.KEYCODE_DPAD_UP, KEYCODE_UP);
        codeMaps.put(KeyEvent.KEYCODE_DPAD_DOWN, KEYCODE_DOWN);
        codeMaps.put(KeyEvent.KEYCODE_DPAD_LEFT, KEYCODE_LEFT);
        codeMaps.put(KeyEvent.KEYCODE_DPAD_RIGHT, KEYCODE_RIGHT);
        codeMaps.put(KeyEvent.KEYCODE_DPAD_CENTER, KEYCODE_ENTER);
        codeMaps.put(KeyEvent.KEYCODE_ENTER, KEYCODE_ENTER);
        codeMaps.put(KeyEvent.KEYCODE_BACK, KEYCODE_BACK);
    }

    /**
     * キーコード
     */
    int jcKeyCode = -1;

    /**
     * イベントタイプ
     */
    int eventType = 0;

    private NativeKeyEvent(KeyEvent event) {
        // イベントタイプの選択
        {
            final int ACTION = event.getAction();
            if (ACTION == KeyEvent.ACTION_DOWN) {
                eventType = EVENT_TYPE_DOWN;
            } else if (ACTION == KeyEvent.ACTION_UP) {
                eventType = EVENT_TYPE_UP;
            }
        }
        // キーコードのマッピング
        {
            final int KEY_CODE = event.getKeyCode();
            if (codeMaps.containsKey(KEY_CODE)) {
                jcKeyCode = codeMaps.get(KEY_CODE);
            } else {
                jcKeyCode = KEYCODE_UNKNOWN;
            }
        }
    }

    @Override
    @JCMethod
    public int getEventType() {
        return eventType;
    }

    @Override
    @JCMethod
    public int getKeyCode() {
        return jcKeyCode;
    }

    /**
     * キーマッピングを追加する
     * @param androidKeyCode Android端末のキーコード
     * @param jointKeyCode 対応するNDK側のキーコード
     */
    public static void addKeyMapping(int androidKeyCode, int jointKeyCode) {
        codeMaps.put(androidKeyCode, jointKeyCode);
    }

    public static NativeKeyEvent createInstance(KeyEvent event) {
        return new NativeKeyEvent(event);
    }
}
