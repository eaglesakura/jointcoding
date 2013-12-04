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
    jc_sp<NDKPlatformContext> platform(new NDKPlatformContext( JointApplicationRenderer::global(_this) ));

    // プラットフォームとのバインドを行う
    application->dispatchBindPlatform(platform);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams(JNIEnv *env, jobject _this, jint main_key, jint sub_key, jintArray params) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams");

    jc_sp<JIntArray> array = JIntArray::wrap(params);

    jint *pParams = array->lock();
    ApplicationQueryKey query(main_key, sub_key);

    // クエリを行う
    jcboolean result = joint_context(_this, JointApplicationBase)->dispatchQueryParams(&query, pParams, array->length());

    // クエリ結果を反映させる
    array->unlock(JniArrayUnlock_Commit);

    return result;
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryStringParams(JNIEnv *env, jobject _this, jint main_key, jint sub_key, jobjectArray params) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryIntParams");

    jc_sp<JObjectArray> array = JObjectArray::wrap(params);

    ApplicationQueryKey query(main_key, sub_key);

    // 一時的な格納先を用意する
    safe_array<String> tempParams;
    tempParams.reserve(array->length());

    // クエリを行う
    jcboolean result = joint_context(_this, JointApplicationBase)->dispatchQueryParams(&query, tempParams.ptr, tempParams.length);

    // 書き込みを行う
    for (int i = 0; i < tempParams.length; ++i) {
        jobject str = (jobject) ndk::c2jstring(tempParams[i].c_str());
        array->set(i, 1, &str);

        CALL_JNIENV();
        env->DeleteLocalRef(str);
    }

    return result;
}

// タッチイベント処理を行う
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_dispatchTouchEvent(JNIEnv *env, jobject _this, jobject toucheventprotocol_0) {

    jc_sp<ndk::NativeTouchEvent> event = ndk::NativeTouchEvent::global(toucheventprotocol_0);
// タッチイベント送信を行う
    joint_context(_this, JointApplicationBase)->dispatchTouchEvent(event);
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postIntParams(JNIEnv *env, jobject _this, jint main_key, jint sub_key, jintArray params) {
// add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postIntParams");

    jc_sp<JIntArray> array = JIntArray::wrap(params);

    jint *pParams = array->lock();
    ApplicationQueryKey query(main_key, sub_key);

// 書き込みを行う
    jcboolean result = joint_context(_this, JointApplicationBase)->dispatchReceiveParams(&query, unsafe_array<s32>(pParams, array->length()));

    array->unlock(JniArrayUnlock_Abort);

    return result;

}

JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postStringParams(JNIEnv *env, jobject _this, jint main_key, jint sub_key, jobjectArray params) {
// add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postStringParams");

    jc_sp<JObjectArray> array = JObjectArray::wrap(params);

    safe_array<String> tempParams;
    tempParams.reserve(array->length());

    for(int i = 0; i < array->length(); ++i) {
        jstring arg = (jstring)array->get(i);
        tempParams[i] = ndk::j2String(arg, jctrue);
        jclogf("post param[%d] = [%s]", i, tempParams[i].c_str());
    }

    ApplicationQueryKey query(main_key, sub_key);

// 書き込みを行う
    jcboolean result = joint_context(_this, JointApplicationBase)->dispatchReceiveParams(&query, tempParams.iterator());

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

