/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeOnClickListener.h"

namespace ndk {

const ::jc::charactor* NativeOnClickListener::CLASS_SIGNATURE = "com/eaglesakura/jc/android/view/listener/NativeOnClickListener";

static jclass class_NativeOnClickListener = NULL;

#define methods_NativeOnClickListener_LENGTH 2

#if methods_NativeOnClickListener_LENGTH
static jmethodID methods_NativeOnClickListener[2];
#endif

static void initialize_NativeOnClickListener() {
    // loaded !
    if (class_NativeOnClickListener) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeOnClickListener = env->FindClass(NativeOnClickListener::CLASS_SIGNATURE);
    class_NativeOnClickListener = (jclass)::ndk::change_globalref(env, class_NativeOnClickListener);
    

    // load methods
    {
        methods_NativeOnClickListener[0] = ::ndk::JniWrapper::loadMethod(class_NativeOnClickListener, "onNativeClick", "(Landroid/view/View;I)V", false);
        methods_NativeOnClickListener[1] = ::ndk::JniWrapper::loadMethod(class_NativeOnClickListener, "setOnClickListener", "(Landroid/view/View;I)Lcom/eaglesakura/jc/android/resource/jni/ObjMonitoring;", true);

    }
}

NativeOnClickListener::NativeOnClickListener(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeOnClickListener();
}

void NativeOnClickListener::onNativeClick(jobject view_0, jint int_1) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_NativeOnClickListener[0], view_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeOnClickListener.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnClickListener_onNativeClick(JNIEnv *env, jobject _this, jobject view_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnClickListener_onNativeClick(JNIEnv *env, jobject _this, jobject view_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_listener_NativeOnClickListener_onNativeClick");
    
    return;
}
#endif


void NativeOnClickListener::onNativeClick_(jobject _this, jobject view_0, jint int_1) {
    CALL_JNIENV();
    initialize_NativeOnClickListener();
    env->CallVoidMethod(_this, methods_NativeOnClickListener[0], view_0, int_1);
}
#if 0
#include "jointcoding-android.h"
#include "ndkNativeOnClickListener.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnClickListener_onNativeClick(JNIEnv *env, jobject _this, jobject view_0, jint int_1);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_view_listener_NativeOnClickListener_onNativeClick(JNIEnv *env, jobject _this, jobject view_0, jint int_1) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_view_listener_NativeOnClickListener_onNativeClick");
    
    return;
}
#endif


jobject NativeOnClickListener::setOnClickListener_unsafe(jobject view, jint nativeListenerPointer) {
    CALL_JNIENV();
    initialize_NativeOnClickListener();
    return (jobject) env->CallStaticObjectMethod(class_NativeOnClickListener, methods_NativeOnClickListener[1], view, nativeListenerPointer);
}

jc_sp<NativeOnClickListener> NativeOnClickListener::wrap(jobject obj) {
    return jc_sp<NativeOnClickListener>( new NativeOnClickListener(obj));
}

jc_sp<NativeOnClickListener> NativeOnClickListener::global(jobject obj) {
    return jc_sp<NativeOnClickListener>( (NativeOnClickListener*)(new NativeOnClickListener(obj))->addGlobalRef());
}

jclass NativeOnClickListener::getClass() {
    initialize_NativeOnClickListener();
    return class_NativeOnClickListener;
}

}

