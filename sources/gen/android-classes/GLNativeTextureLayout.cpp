/**
 * export from "Joint Coding Project"
 */
#include "GLNativeTextureLayout.h"

const ::jc::charactor* GLNativeTextureLayout::CLASS_SIGNATURE = "com/eaglesakura/jc/android/view/GLNativeTextureLayout";

static jclass class_GLNativeTextureLayout = NULL;

#define methods_GLNativeTextureLayout_LENGTH 0

#if methods_GLNativeTextureLayout_LENGTH
static jmethodID methods_GLNativeTextureLayout[0];
#endif

static void initialize_GLNativeTextureLayout() {
    // loaded !
    if (class_GLNativeTextureLayout) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_GLNativeTextureLayout = env->FindClass(GLNativeTextureLayout::CLASS_SIGNATURE);
    class_GLNativeTextureLayout = (jclass)::ndk::change_globalref(env, class_GLNativeTextureLayout);
    

    // load methods
    {

    }
}

GLNativeTextureLayout::GLNativeTextureLayout(jobject obj): ::ndk::JniWrapper(obj){
    initialize_GLNativeTextureLayout();
}

jc_sp<GLNativeTextureLayout> GLNativeTextureLayout::wrap(jobject obj) {
    return jc_sp<GLNativeTextureLayout>( new GLNativeTextureLayout(obj));
}

jc_sp<GLNativeTextureLayout> GLNativeTextureLayout::global(jobject obj) {
    return jc_sp<GLNativeTextureLayout>( (GLNativeTextureLayout*)(new GLNativeTextureLayout(obj))->addGlobalRef());
}

jclass GLNativeTextureLayout::getClass() {
    initialize_GLNativeTextureLayout();
    return class_GLNativeTextureLayout;
}
