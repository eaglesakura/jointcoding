/*
 * JointApplicationRenderer_impl.cpp
 *
 *  Created on: 2013/05/07
 */

#include "jointcoding-android.h"
#include "android-classes/ndkJointApplicationRenderer.h"

#include    "jc/framework/app/JointApplicationBase.h"
#include    "jcandroid/egl/sdk/SdkDeviceManagerContext.hpp"
#include    "jcandroid/framework/NDKPlatformContext.h"
#include    "android-classes/ndkNativeTouchEvent.h"

using namespace ndk;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize(JNIEnv *env, jobject _this) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeInitialize");
    // JointAppを先に取り出しておく
    JointApplicationBase *application = joint_context(_this, JointApplicationBase);

    // プラットフォームを生成する
    jc_sp<NDKPlatformContext> platform(mark_new NDKPlatformContext( JointApplicationRenderer::global(_this) ));

    // プラットフォームとのバインドを行う
    application->dispatchBindPlatform(platform);
}

// タッチイベント処理を行う
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_dispatchTouchEvent(JNIEnv *env, jobject _this, jobject toucheventprotocol_0) {

    jc_sp<ndk::NativeTouchEvent> event = ndk::NativeTouchEvent::global(toucheventprotocol_0);
// タッチイベント送信を行う
    joint_context(_this, JointApplicationBase)->dispatchTouchEvent(event);
}

JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams(JNIEnv *env, jobject _this, jint main_key, jint sub_key, jobjectArray params) {
    safe_array<String> tempParams;
    if(params) {
        tempParams.reserve(env->GetArrayLength(params));
    }
    JointApplicationBase::string_params iterator = tempParams.iterator();
    if(params) {
        j2stringArray(params, &iterator);
    }

    ApplicationQueryKey query(main_key, sub_key);

// 書き込みを行う
    const jcboolean result = joint_context(_this, JointApplicationBase)->dispatchReceiveParams(&query, iterator);

    if(result && params) {
        c2stringArray(params, tempParams.ptr, tempParams.length);
    }

    return result;
}
// 非同期タスクの開始を行う
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeNewtask(JNIEnv *env, jobject _this, jint uniqueId, jint user_data) {
    ApplicationTaskContext task;
    task.uniqueId = uniqueId;
    task.user_data = user_data;
    joint_context(_this, JointApplicationBase)->dispatchNewTask(task);
}

}

