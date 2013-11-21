/**
 * export from "Joint Coding Project"
 */
#include "ndkAndroidUtil.h"

namespace ndk {

const ::jc::charactor* AndroidUtil::CLASS_SIGNATURE = "com/eaglesakura/jc/android/app/AndroidUtil";

static jclass class_AndroidUtil = NULL;

#define methods_AndroidUtil_LENGTH 2

#if methods_AndroidUtil_LENGTH
static jmethodID methods_AndroidUtil[2];
#endif

static void initialize_AndroidUtil() {
    // loaded !
    if (class_AndroidUtil) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_AndroidUtil = env->FindClass(AndroidUtil::CLASS_SIGNATURE);
    class_AndroidUtil = (jclass)::ndk::change_globalref(env, class_AndroidUtil);
    

    // load methods
    {
        methods_AndroidUtil[0] = ::ndk::JniWrapper::loadMethod(class_AndroidUtil, "_log", "(Ljava/lang/String;)V", true);
        methods_AndroidUtil[1] = ::ndk::JniWrapper::loadMethod(class_AndroidUtil, "getSurface", "(Landroid/view/SurfaceHolder;)Landroid/view/Surface;", true);

    }
}

AndroidUtil::AndroidUtil(jobject obj): ::ndk::JniWrapper(obj){
    initialize_AndroidUtil();
}

void AndroidUtil::_log(jstring string_0) {
    CALL_JNIENV();
    initialize_AndroidUtil();
    env->CallStaticVoidMethod(class_AndroidUtil, methods_AndroidUtil[0], string_0);
}
#if 0
#include "jointcoding-android.h"
#include "ndkAndroidUtil.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_AndroidUtil__log(JNIEnv *env, jobject _this, jstring string_0);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_app_AndroidUtil__log(JNIEnv *env, jobject _this, jstring string_0) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_app_AndroidUtil__log");
    
    return;
}
#endif


jobject AndroidUtil::getSurface_unsafe(jobject holder) {
    CALL_JNIENV();
    initialize_AndroidUtil();
    return (jobject) env->CallStaticObjectMethod(class_AndroidUtil, methods_AndroidUtil[1], holder);
}

jc_sp<AndroidUtil> AndroidUtil::wrap(jobject obj) {
    return jc_sp<AndroidUtil>( mark_new AndroidUtil(obj));
}

jc_sp<AndroidUtil> AndroidUtil::global(jobject obj) {
    return jc_sp<AndroidUtil>( (AndroidUtil*)(mark_new AndroidUtil(obj))->addGlobalRef());
}

jclass AndroidUtil::getClass() {
    initialize_AndroidUtil();
    return class_AndroidUtil;
}

}

