/**
 * export from "Joint Coding Project"
 */
#include "ndkImageEncoder.h"

namespace ndk {

const ::jc::charactor* ImageEncoder::CLASS_SIGNATURE = "com/eaglesakura/jc/jni/image/ImageEncoder";

static jclass class_ImageEncoder = NULL;

#define methods_ImageEncoder_LENGTH 4

#if methods_ImageEncoder_LENGTH
static jmethodID methods_ImageEncoder[4];
#endif

static void initialize_ImageEncoder() {
    // loaded !
    if (class_ImageEncoder) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_ImageEncoder = env->FindClass(ImageEncoder::CLASS_SIGNATURE);
    class_ImageEncoder = (jclass)::ndk::change_globalref(env, class_ImageEncoder);
    

    // load methods
    {
        methods_ImageEncoder[0] = ::ndk::JniWrapper::loadMethod(class_ImageEncoder, "alloc", "(I)Ljava/nio/Buffer;", false);
        methods_ImageEncoder[1] = ::ndk::JniWrapper::loadMethod(class_ImageEncoder, "encodeRGBA", "()V", false);
        methods_ImageEncoder[2] = ::ndk::JniWrapper::loadMethod(class_ImageEncoder, "createInstance", "()Lcom/eaglesakura/jc/jni/image/ImageEncoder;", true);
        methods_ImageEncoder[3] = ::ndk::JniWrapper::loadMethod(class_ImageEncoder, "setImageSize", "(II)V", false);

    }
}

ImageEncoder::ImageEncoder(jobject obj): ::ndk::JniWrapper(obj){
    initialize_ImageEncoder();
}

jobject ImageEncoder::alloc_unsafe(jint bufferSize) {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_ImageEncoder[0], bufferSize);
}

jobject ImageEncoder::alloc_unsafe_(jobject _this, jint bufferSize) {
    CALL_JNIENV();
    initialize_ImageEncoder();
    return (jobject) env->CallObjectMethod(_this, methods_ImageEncoder[0], bufferSize);
}

void ImageEncoder::encodeRGBA() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_ImageEncoder[1]);
}

void ImageEncoder::encodeRGBA_(jobject _this) {
    CALL_JNIENV();
    initialize_ImageEncoder();
    env->CallVoidMethod(_this, methods_ImageEncoder[1]);
}

jobject ImageEncoder::createInstance_unsafe() {
    CALL_JNIENV();
    initialize_ImageEncoder();
    return (jobject) env->CallStaticObjectMethod(class_ImageEncoder, methods_ImageEncoder[2]);
}

void ImageEncoder::setImageSize(jint width, jint height) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_ImageEncoder[3], width, height);
}

void ImageEncoder::setImageSize_(jobject _this, jint width, jint height) {
    CALL_JNIENV();
    initialize_ImageEncoder();
    env->CallVoidMethod(_this, methods_ImageEncoder[3], width, height);
}

jc_sp<ImageEncoder> ImageEncoder::wrap(jobject obj) {
    return jc_sp<ImageEncoder>( new ImageEncoder(obj));
}

jc_sp<ImageEncoder> ImageEncoder::global(jobject obj) {
    return jc_sp<ImageEncoder>( (ImageEncoder*)(new ImageEncoder(obj))->addGlobalRef());
}

jclass ImageEncoder::getClass() {
    initialize_ImageEncoder();
    return class_ImageEncoder;
}

}

