/*
 * TestSupport.cpp
 *
 *  Created on: 2012/07/06
 */
#include "jointcoding-android.h"
#include "ndkTestSupport.h"
#include "ndkNDK.h"
#include    "dlfcn.h"

namespace jc {

/**
 * trueであることを宣言する
 */
void __assertTrue(const charactor* file, const s32 line, const bool is, bool *success, const charactor* message) {
    if (!is) {
        // falseは実行を停止させる
        CALL_JNIENV();
        jclogf("assert error!! from %s - L %d || %s", file, line, message);
        env->ThrowNew(jc::AssertError::getClass(), message);
        (*success) = false;
        return;
    }

    (*success) = true;
}

jcboolean TestSupport::callTestFnction(const charactor* packageName, const charactor* libName, const charactor* functionName) {

    String path = "/data/data/";
    path += packageName;
    path += "/lib";
    path += libName;
    path += ".so";
    void* so = dlopen(path.c_str(), RTLD_NOW);
    if (so) {
        test_function_ptr func = (test_function_ptr) dlsym(so, functionName);
        if (func) {
            // 関数を呼び出す
            func();
            dlclose(so);
        } else {
            // 関数が見つからなかった
            jclogf("function not found = %s", functionName);
            dlclose(so);
            return jcfalse;
        }
    } else {
        jclogf("sofile not load = %s", path.c_str());
        return jcfalse;
    }

    return jctrue;
}

}
