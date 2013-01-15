/*
 * ndkNativeClickListener.cpp
 *
 *  Created on: 2012/10/06
 */

#include "jointcoding-android.h"
#include "android-classes/ndkNativeOnClickListener.h"
#include "jc/view/OnClickListener.h"

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnClickListener_onNativeClick(JNIEnv *env, jobject _this, jobject view, jc::ViewOnClickListener *pListener) {
    // call env reset
    initJniEnv(env);

    // コールバック
    pListener->onClick();

    return;
}

}
