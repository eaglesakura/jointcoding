/**
 * export from "Joint Coding Project"
 */
#include "ndkAppFont.h"

namespace ndk {

const ::jc::charactor* AppFont::CLASS_SIGNATURE = "com/eaglesakura/jc/font/AppFont";

static jclass class_AppFont = NULL;

#define methods_AppFont_LENGTH 4

#if methods_AppFont_LENGTH
static jmethodID methods_AppFont[4];
#endif

static void initialize_AppFont() {
    // loaded !
    if (class_AppFont) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_AppFont = env->FindClass(AppFont::CLASS_SIGNATURE);
    class_AppFont = (jclass)::ndk::change_globalref(env, class_AppFont);
    

    // load methods
    {
        methods_AppFont[0] = ::ndk::JniWrapper::loadMethod(class_AppFont, "createInstance", "()Lcom/eaglesakura/jc/font/AppFont;", true);
        methods_AppFont[1] = ::ndk::JniWrapper::loadMethod(class_AppFont, "createRawImage", "(Ljava/lang/String;I)Lcom/eaglesakura/jc/jni/image/ImageDecoder;", true);
        methods_AppFont[2] = ::ndk::JniWrapper::loadMethod(class_AppFont, "createImage", "(Ljava/lang/String;I)V", false);
        methods_AppFont[3] = ::ndk::JniWrapper::loadMethod(class_AppFont, "getImage", "()Landroid/graphics/Bitmap;", false);

    }
}

AppFont::AppFont(jobject obj): ::ndk::JniWrapper(obj){
    initialize_AppFont();
}

jobject AppFont::createInstance_unsafe() {
    CALL_JNIENV();
    initialize_AppFont();
    return (jobject) env->CallStaticObjectMethod(class_AppFont, methods_AppFont[0]);
}

jobject AppFont::createRawImage_unsafe(jstring text, jint height) {
    CALL_JNIENV();
    initialize_AppFont();
    return (jobject) env->CallStaticObjectMethod(class_AppFont, methods_AppFont[1], text, height);
}

void AppFont::createImage(jstring text, jint heightPixel) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_AppFont[2], text, heightPixel);
}

void AppFont::createImage_(jobject _this, jstring text, jint heightPixel) {
    CALL_JNIENV();
    initialize_AppFont();
    env->CallVoidMethod(_this, methods_AppFont[2], text, heightPixel);
}

jobject AppFont::getImage_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_AppFont[3]);
}

jobject AppFont::getImage_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_AppFont();
    return (jobject) env->CallObjectMethod(_this, methods_AppFont[3]);
}

jc_sp<AppFont> AppFont::wrap(jobject obj) {
    return jc_sp<AppFont>( new AppFont(obj));
}

jc_sp<AppFont> AppFont::global(jobject obj) {
    return jc_sp<AppFont>( (AppFont*)(new AppFont(obj))->addGlobalRef());
}

jclass AppFont::getClass() {
    initialize_AppFont();
    return class_AppFont;
}

}

