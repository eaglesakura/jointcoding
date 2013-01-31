/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeOnTouchListener.h"

namespace ndk {

const ::jc::charactor* NativeOnTouchListener::CLASS_SIGNATURE = "com/eaglesakura/jc/android/view/listener/NativeOnTouchListener";

static jclass class_NativeOnTouchListener = NULL;

#define methods_NativeOnTouchListener_LENGTH 2

#if methods_NativeOnTouchListener_LENGTH
static jmethodID methods_NativeOnTouchListener[2];
#endif

static void initialize_NativeOnTouchListener() {
    // loaded !
    if (class_NativeOnTouchListener) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeOnTouchListener = env->FindClass(NativeOnTouchListener::CLASS_SIGNATURE);
    class_NativeOnTouchListener = (jclass)::ndk::change_globalref(env, class_NativeOnTouchListener);
    

    // load methods
    {
        methods_NativeOnTouchListener[0] = ::ndk::JniWrapper::loadMethod(class_NativeOnTouchListener, "onNativeTouchEvent", "(Lcom/eaglesakura/jc/android/ui/NativeTouchEvent;III)V", false);
        methods_NativeOnTouchListener[1] = ::ndk::JniWrapper::loadMethod(class_NativeOnTouchListener, "setOnTouchListener", "(Landroid/view/View;I)Lcom/eaglesakura/jc/android/resource/jni/ObjMonitoring;", true);

    }
}

NativeOnTouchListener::NativeOnTouchListener(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeOnTouchListener();
}

void NativeOnTouchListener::onNativeTouchEvent(jobject nativetouchevent_0, jint int_1, jint int_2, jint int_3) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeOnTouchListener[0], nativetouchevent_0, int_1, int_2, int_3);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeOnTouchListener.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnTouchListener_onNativeTouchEvent(JNIEnv *env, jobject _this, jobject nativetouchevent_0, jint int_1, jint int_2, jint int_3);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnTouchListener_onNativeTouchEvent(JNIEnv *env, jobject _this, jobject nativetouchevent_0, jint int_1, jint int_2, jint int_3) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_listener_NativeOnTouchListener_onNativeTouchEvent");
    
    return;
}
#endif


void NativeOnTouchListener::onNativeTouchEvent_(jobject _this, jobject nativetouchevent_0, jint int_1, jint int_2, jint int_3) {
    CALL_JNIENV();
    initialize_NativeOnTouchListener();
    env->CallVoidMethod(_this, methods_NativeOnTouchListener[0], nativetouchevent_0, int_1, int_2, int_3);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeOnTouchListener.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnTouchListener_onNativeTouchEvent(JNIEnv *env, jobject _this, jobject nativetouchevent_0, jint int_1, jint int_2, jint int_3);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnTouchListener_onNativeTouchEvent(JNIEnv *env, jobject _this, jobject nativetouchevent_0, jint int_1, jint int_2, jint int_3) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_listener_NativeOnTouchListener_onNativeTouchEvent");
    
    return;
}
#endif


jobject NativeOnTouchListener::setOnTouchListener_unsafe(jobject view, jint nativeListenerPointer) {
    CALL_JNIENV();
    initialize_NativeOnTouchListener();
    return (jobject) env->CallStaticObjectMethod(class_NativeOnTouchListener, methods_NativeOnTouchListener[1], view, nativeListenerPointer);
}

jc_sp<NativeOnTouchListener> NativeOnTouchListener::wrap(jobject obj) {
    return jc_sp<NativeOnTouchListener>( new NativeOnTouchListener(obj));
}

jc_sp<NativeOnTouchListener> NativeOnTouchListener::global(jobject obj) {
    return jc_sp<NativeOnTouchListener>( (NativeOnTouchListener*)(new NativeOnTouchListener(obj))->addGlobalRef());
}

jclass NativeOnTouchListener::getClass() {
    initialize_NativeOnTouchListener();
    return class_NativeOnTouchListener;
}

}

