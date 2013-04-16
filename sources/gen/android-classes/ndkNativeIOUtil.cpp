/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeIOUtil.h"

namespace ndk {

const ::jc::charactor* NativeIOUtil::CLASS_SIGNATURE = "com/eaglesakura/jc/android/app/util/NativeIOUtil";

static jclass class_NativeIOUtil = NULL;

#define methods_NativeIOUtil_LENGTH 3

#if methods_NativeIOUtil_LENGTH
static jmethodID methods_NativeIOUtil[3];
#endif

static void initialize_NativeIOUtil() {
    // loaded !
    if (class_NativeIOUtil) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeIOUtil = env->FindClass(NativeIOUtil::CLASS_SIGNATURE);
    class_NativeIOUtil = (jclass)::ndk::change_globalref(env, class_NativeIOUtil);
    

    // load methods
    {
        methods_NativeIOUtil[0] = ::ndk::JniWrapper::loadMethod(class_NativeIOUtil, "openFromAssets", "(Ljava/lang/String;Landroid/content/Context;)Ljava/io/InputStream;", true);
        methods_NativeIOUtil[1] = ::ndk::JniWrapper::loadMethod(class_NativeIOUtil, "openFromLocalStrage", "(Ljava/lang/String;Landroid/content/Context;)Ljava/io/InputStream;", true);
        methods_NativeIOUtil[2] = ::ndk::JniWrapper::loadMethod(class_NativeIOUtil, "openFromExternalStrage", "(Ljava/lang/String;Landroid/content/Context;)Ljava/io/InputStream;", true);

    }
}

NativeIOUtil::NativeIOUtil(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeIOUtil();
}

jobject NativeIOUtil::openFromAssets_unsafe(jstring fileName, jobject appContext) {
    CALL_JNIENV();
    initialize_NativeIOUtil();
    return (jobject) env->CallStaticObjectMethod(class_NativeIOUtil, methods_NativeIOUtil[0], fileName, appContext);
}

jobject NativeIOUtil::openFromLocalStrage_unsafe(jstring path, jobject appContext) {
    CALL_JNIENV();
    initialize_NativeIOUtil();
    return (jobject) env->CallStaticObjectMethod(class_NativeIOUtil, methods_NativeIOUtil[1], path, appContext);
}

jobject NativeIOUtil::openFromExternalStrage_unsafe(jstring path, jobject appContext) {
    CALL_JNIENV();
    initialize_NativeIOUtil();
    return (jobject) env->CallStaticObjectMethod(class_NativeIOUtil, methods_NativeIOUtil[2], path, appContext);
}

jc_sp<NativeIOUtil> NativeIOUtil::wrap(jobject obj) {
    return jc_sp<NativeIOUtil>( new NativeIOUtil(obj));
}

jc_sp<NativeIOUtil> NativeIOUtil::global(jobject obj) {
    return jc_sp<NativeIOUtil>( (NativeIOUtil*)(new NativeIOUtil(obj))->addGlobalRef());
}

jclass NativeIOUtil::getClass() {
    initialize_NativeIOUtil();
    return class_NativeIOUtil;
}

}

