/*
 * ndkNativeOnTouchListener_impl.cpp
 *
 *  Created on: 2012/10/05
 */

#include "jointcoding-android.h"
#include "android-classes/ndkNativeOnTouchListener.h"
#include "android-classes/ndkNativeTouchEvent.h"
#include "jc/view/OnTouchListener.h"

using namespace ndk;
extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnTouchListener_onNativeTouchEvent(JNIEnv *env, jobject _this, jobject nev, ViewOnTouchListener *pListener, jint index, jint num) {
    // call env reset
    initJniEnv(env);

    // add code.
    jc_sp<NativeTouchEvent> event = NativeTouchEvent::wrap(nev);

    if (index == 0) {
        // 初回のみ、beginを発生させる
        pListener->onTouchEventBegin(num);
    }

    // イベントを飛ばす
    pListener->onTouchEvent(event, index, num);

    // 最後ならendイベントをつける
    if (index == (num - 1)) {
        pListener->onTouchEventComplete();
    }

    return;
}

}

