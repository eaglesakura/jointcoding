/*
 * JniTestSupport_jni.cpp
 *
 *  Created on: 2012/07/06
 */

#include "jointcoding-android.h"
#include "jointcoding-testsupport.h"
#include "jcandroid/util/Utils.h"
#include "jcJniTestSupport.h"
#include    "ndkTestSupport.h"

using namespace jc;
using namespace ndk;

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_testsupport_JniTestSupport_callJniTest(JNIEnv *env, jobject _this, jstring packageName, jstring libName, jstring funcName) {
    // call env reset
    initJniEnv(env);

    String sPackageName = j2String(packageName);
    String sLibName = j2String(libName);
    String sFuncName = j2String(funcName);

    if (!TestSupport::callTestFnction(sPackageName.c_str(), sLibName.c_str(), sFuncName.c_str())) {
        env->ThrowNew(AssertError::getClass(), "function not found!!");
        return;
    }

    return;
}

}

