/**
 * export from "Joint Coding Project"
 */
#include "jcAssertError.h"

namespace jc {

const ::jc::charactor* AssertError::CLASS_SIGNATURE = "com/eaglesakura/jc/android/testsupport/AssertError";

static jclass class_AssertError = NULL;

#define methods_AssertError_LENGTH 0

#if methods_AssertError_LENGTH
static jmethodID methods_AssertError[0];
#endif

static void initialize_AssertError() {
    // loaded !
    if (class_AssertError) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_AssertError = env->FindClass(AssertError::CLASS_SIGNATURE);
    class_AssertError = (jclass)::ndk::change_globalref(env, class_AssertError);
    

    // load methods
    {

    }
}

AssertError::AssertError(jobject obj): ::ndk::JniWrapper(obj){
    initialize_AssertError();
}

jc_sp<AssertError> AssertError::wrap(jobject obj) {
    return jc_sp<AssertError>( new AssertError(obj));
}

jc_sp<AssertError> AssertError::global(jobject obj) {
    return jc_sp<AssertError>( (AssertError*)(new AssertError(obj))->addGlobalRef());
}

jclass AssertError::getClass() {
    initialize_AssertError();
    return class_AssertError;
}

}

