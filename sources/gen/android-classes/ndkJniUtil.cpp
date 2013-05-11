/**
 * export from "Joint Coding Project"
 */
#include "ndkJniUtil.h"

namespace ndk {

const ::jc::charactor* JniUtil::CLASS_SIGNATURE = "com/eaglesakura/jc/jni/JniUtil";

static jclass class_JniUtil = NULL;

#define methods_JniUtil_LENGTH 2

#if methods_JniUtil_LENGTH
static jmethodID methods_JniUtil[2];
#endif

static void initialize_JniUtil() {
    // loaded !
    if (class_JniUtil) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JniUtil = env->FindClass(JniUtil::CLASS_SIGNATURE);
    class_JniUtil = (jclass)::ndk::change_globalref(env, class_JniUtil);
    

    // load methods
    {
        methods_JniUtil[0] = ::ndk::JniWrapper::loadMethod(class_JniUtil, "getAPILevel", "()I", true);
        methods_JniUtil[1] = ::ndk::JniWrapper::loadMethod(class_JniUtil, "printStackTrace", "(Ljava/lang/Exception;)V", true);

    }
}

JniUtil::JniUtil(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JniUtil();
}

jint JniUtil::getAPILevel() {
    CALL_JNIENV();
    initialize_JniUtil();
    return (jint) env->CallStaticIntMethod(class_JniUtil, methods_JniUtil[0]);
}

void JniUtil::printStackTrace(jobject e) {
    CALL_JNIENV();
    initialize_JniUtil();
    env->CallStaticVoidMethod(class_JniUtil, methods_JniUtil[1], e);
}

jc_sp<JniUtil> JniUtil::wrap(jobject obj) {
    return jc_sp<JniUtil>( new JniUtil(obj));
}

jc_sp<JniUtil> JniUtil::global(jobject obj) {
    return jc_sp<JniUtil>( (JniUtil*)(new JniUtil(obj))->addGlobalRef());
}

jclass JniUtil::getClass() {
    initialize_JniUtil();
    return class_JniUtil;
}

}

