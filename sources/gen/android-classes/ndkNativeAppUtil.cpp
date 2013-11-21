/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeAppUtil.h"

namespace ndk {

const ::jc::charactor* NativeAppUtil::CLASS_SIGNATURE = "com/eaglesakura/jc/android/app/util/NativeAppUtil";

static jclass class_NativeAppUtil = NULL;

#define methods_NativeAppUtil_LENGTH 2

#if methods_NativeAppUtil_LENGTH
static jmethodID methods_NativeAppUtil[2];
#endif

static void initialize_NativeAppUtil() {
    // loaded !
    if (class_NativeAppUtil) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeAppUtil = env->FindClass(NativeAppUtil::CLASS_SIGNATURE);
    class_NativeAppUtil = (jclass)::ndk::change_globalref(env, class_NativeAppUtil);
    

    // load methods
    {
        methods_NativeAppUtil[0] = ::ndk::JniWrapper::loadMethod(class_NativeAppUtil, "getAPILevel", "()I", true);
        methods_NativeAppUtil[1] = ::ndk::JniWrapper::loadMethod(class_NativeAppUtil, "printStackTrace", "(Ljava/lang/Exception;)V", true);

    }
}

NativeAppUtil::NativeAppUtil(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeAppUtil();
}

jint NativeAppUtil::getAPILevel() {
    CALL_JNIENV();
    initialize_NativeAppUtil();
    return (jint) env->CallStaticIntMethod(class_NativeAppUtil, methods_NativeAppUtil[0]);
}

void NativeAppUtil::printStackTrace(jobject e) {
    CALL_JNIENV();
    initialize_NativeAppUtil();
    env->CallStaticVoidMethod(class_NativeAppUtil, methods_NativeAppUtil[1], e);
}

jc_sp<NativeAppUtil> NativeAppUtil::wrap(jobject obj) {
    return jc_sp<NativeAppUtil>( mark_new NativeAppUtil(obj));
}

jc_sp<NativeAppUtil> NativeAppUtil::global(jobject obj) {
    return jc_sp<NativeAppUtil>( (NativeAppUtil*)(mark_new NativeAppUtil(obj))->addGlobalRef());
}

jclass NativeAppUtil::getClass() {
    initialize_NativeAppUtil();
    return class_NativeAppUtil;
}

}

