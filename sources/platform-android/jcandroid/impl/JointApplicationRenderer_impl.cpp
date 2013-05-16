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
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_queryParams(JNIEnv *env, jobject _this, jint main_key, jint sub_key, jintArray params) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams");

    jc_sp<JIntArray> array = JIntArray::wrap(params);

    jint *pParams = array->lock();
    ApplicationQueryKey query(main_key, sub_key);

    // クエリを行う
    jcboolean result = joint_context(_this, JointApplicationBase)->queryParams(&query, pParams, array->length());

    // クエリ結果を反映させる
    array->unlock(JniArrayUnlock_Commit);

    return result;
}

// main
JNIEXPORT jboolean JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams(JNIEnv *env, jobject _this, jint main_key, jint sub_key, jintArray params) {
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_postParams");

    jc_sp<JIntArray> array = JIntArray::wrap(params);

    const jint *pParams = array->lock();
    ApplicationQueryKey query(main_key, sub_key);

    // 書き込みを行う
    jcboolean result = joint_context(_this, JointApplicationBase)->postParams(&query, pParams, array->length());

    array->unlock(JniArrayUnlock_Abort);

    return result;

}
// 非同期タスクの開始を行う
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_framework_app_JointApplicationRenderer_onNativeNewtask(JNIEnv *env, jobject _this, jint uniqueId, jint user_data) {
    ApplicationTaskContext task;
    task.uniqueId = uniqueId;
    task.user_data = user_data;
    joint_context(_this, JointApplicationBase)->dispatchTask(task);
}

}

