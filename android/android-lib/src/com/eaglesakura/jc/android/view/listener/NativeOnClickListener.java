package com.eaglesakura.jc.android.view.listener;

import android.view.View;
import android.view.View.OnClickListener;

import com.eaglesakura.jc.android.IJointCodingClass;
import com.eaglesakura.jc.android.resource.jni.ObjMonitoring;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * クリックイベントを取り出すためのリスナ
 * ポインタは常に {@link ObjMonitoring} によってモニタされ、native側の監視機構(ndk::SystemMemory)で監視される。
 *
 */
@JCClass(
         cppNamespace = "ndk")
public class NativeOnClickListener implements OnClickListener, IJointCodingClass {
    int nativeListenerPointer;

    private NativeOnClickListener(int pointer) {
        nativeListenerPointer = pointer;
    }

    @Override
    public final void onClick(View v) {
        onNativeClick(v, nativeListenerPointer);
    }

    @JCMethod(
              nativeMethod = true)
    native void onNativeClick(View v, int pointer);

    /**
     * リスナを作成して、Viewに登録する。
     * @param view
     * @param nativeListenerPointer
     * @return リスナのモニター。リスナ自体のgcは行えないが、別途監視して後々gc用に利用する。
     */
    @JCMethod
    public static ObjMonitoring setOnClickListener(View view, int nativeListenerPointer) {
        NativeOnClickListener listener = new NativeOnClickListener(nativeListenerPointer);
        view.setOnClickListener(listener);
        return new ObjMonitoring(listener);
    }
}
