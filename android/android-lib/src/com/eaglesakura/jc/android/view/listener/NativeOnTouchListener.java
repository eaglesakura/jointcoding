package com.eaglesakura.jc.android.view.listener;

import java.util.List;

import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

import com.eaglesakura.jc.android.resource.jni.ObjMonitoring;
import com.eaglesakura.jc.android.ui.NativeTouchEvent;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * タッチイベントを取り出すためのリスナ
 * ポインタは常に {@link ObjMonitoring} によってモニタされ、native側の監視機構(SystemMemory)で監視される。
 *
 */
@JCClass(
         cppNamespace = "ndk")
public class NativeOnTouchListener implements OnTouchListener {
    int nativeListenerPointer;

    private NativeOnTouchListener(int nativeListener) {
        this.nativeListenerPointer = nativeListener;
    }

    @Override
    public final boolean onTouch(View v, MotionEvent event) {
        List<NativeTouchEvent> events = NativeTouchEvent.toNativeEvents(event);

        final int num = events.size();
        int index = 0;
        for (NativeTouchEvent nev : events) {
            onNativeTouchEvent(nev, nativeListenerPointer, index, num);
            ++index;
        }
        return true;
    }

    /**
     * nativeにタッチイベントを伝えて処理してもらう。
     * @param naEvent イベント本体
     * @param pListener ネイティブポインタ値
     * @param index イベントインデックス
     * @param num イベント数
     */
    @JCMethod(
              nativeMethod = true)
    native void onNativeTouchEvent(NativeTouchEvent naEvent, int pListener, int index, int num);

    /**
     * リスナを作成して、Viewに登録する。
     * @param view
     * @param nativeListenerPointer
     * @return リスナのモニター。リスナ自体のgcは行えないが、別途監視して後々gc用に利用する。
     */
    @JCMethod
    public static ObjMonitoring setOnTouchListener(View view, int nativeListenerPointer) {
        NativeOnTouchListener listener = new NativeOnTouchListener(nativeListenerPointer);
        view.setOnTouchListener(listener);
        return new ObjMonitoring(listener);
    }
}
