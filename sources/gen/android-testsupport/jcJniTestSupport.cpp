/**
 * export from "Joint Coding Project"
 */
#include "jcJniTestSupport.h"

namespace jc {

const ::jc::charactor* JniTestSupport::CLASS_SIGNATURE = "com/eaglesakura/jc/android/testsupport/JniTestSupport";

static jclass class_JniTestSupport = NULL;

#define methods_JniTestSupport_LENGTH 1

#if methods_JniTestSupport_LENGTH
static jmethodID methods_JniTestSupport[1];
#endif

static void initialize_JniTestSupport() {
    // loaded !
    if (class_JniTestSupport) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_JniTestSupport = env->FindClass(JniTestSupport::CLASS_SIGNATURE);
    class_JniTestSupport = (jclass)::ndk::change_globalref(env, class_JniTestSupport);
    

    // load methods
    {
        methods_JniTestSupport[0] = ::ndk::JniWrapper::loadMethod(class_JniTestSupport, "callJniTest", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V", true);

    }
}

JniTestSupport::JniTestSupport(jobject obj): ::ndk::JniWrapper(obj){
    initialize_JniTestSupport();
}

void JniTestSupport::callJniTest(jstring string_0, jstring string_1, jstring string_2) {
    CALL_JNIENV();
    initialize_JniTestSupport();
    env->CallStaticVoidMethod(class_JniTestSupport, methods_JniTestSupport[0], string_0, string_1, string_2);
}
#if 0
#include "jointcoding-android.h"
#include "jcJniTestSupport.h"

extern "C" {
// prototype
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_testsupport_JniTestSupport_callJniTest(JNIEnv *env, jobject _this, jstring string_0, jstring string_1, jstring string_2);
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_testsupport_JniTestSupport_callJniTest(JNIEnv *env, jobject _this, jstring string_0, jstring string_1, jstring string_2) {
    // call env reset
    initJniEnv(env);
    
    // add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_android_testsupport_JniTestSupport_callJniTest");
    
    return;
}
#endif


jc_sp<JniTestSupport> JniTestSupport::wrap(jobject obj) {
    return jc_sp<JniTestSupport>( new JniTestSupport(obj));
}

jc_sp<JniTestSupport> JniTestSupport::global(jobject obj) {
    return jc_sp<JniTestSupport>( (JniTestSupport*)(new JniTestSupport(obj))->addGlobalRef());
}

jclass JniTestSupport::getClass() {
    initialize_JniTestSupport();
    return class_JniTestSupport;
}

}

