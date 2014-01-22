/**
 * export from "Joint Coding Project"
 */
#include "ndkImageDecoder.h"

namespace ndk {

const ::jc::charactor* ImageDecoder::CLASS_SIGNATURE = "com/eaglesakura/jc/android/app/util/ImageDecoder";

static jclass class_ImageDecoder = NULL;

#define methods_ImageDecoder_LENGTH 7

#if methods_ImageDecoder_LENGTH
static jmethodID methods_ImageDecoder[7];
#endif

static void initialize_ImageDecoder() {
    // loaded !
    if (class_ImageDecoder) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_ImageDecoder = env->FindClass(ImageDecoder::CLASS_SIGNATURE);
    class_ImageDecoder = (jclass)::ndk::change_globalref(env, class_ImageDecoder);
    

    // load methods
    {
        methods_ImageDecoder[0] = ::ndk::JniWrapper::loadMethod(class_ImageDecoder, "getPixels", "()Ljava/nio/Buffer;", false);
        methods_ImageDecoder[1] = ::ndk::JniWrapper::loadMethod(class_ImageDecoder, "decodeBitmapFromNativeUri", "(Ljava/lang/String;)Landroid/graphics/Bitmap;", true);
        methods_ImageDecoder[2] = ::ndk::JniWrapper::loadMethod(class_ImageDecoder, "getHeight", "()I", false);
        methods_ImageDecoder[3] = ::ndk::JniWrapper::loadMethod(class_ImageDecoder, "decodeFromStream", "(Ljava/io/InputStream;)Lcom/eaglesakura/jc/android/app/util/ImageDecoder;", true);
        methods_ImageDecoder[4] = ::ndk::JniWrapper::loadMethod(class_ImageDecoder, "decodeBitmapFromStream", "(Ljava/io/InputStream;)Landroid/graphics/Bitmap;", true);
        methods_ImageDecoder[5] = ::ndk::JniWrapper::loadMethod(class_ImageDecoder, "getWidth", "()I", false);
        methods_ImageDecoder[6] = ::ndk::JniWrapper::loadMethod(class_ImageDecoder, "decodeFromBitmap", "(Landroid/graphics/Bitmap;Z)Lcom/eaglesakura/jc/android/app/util/ImageDecoder;", true);

    }
}

ImageDecoder::ImageDecoder(jobject obj): ::ndk::JniWrapper(obj){
    initialize_ImageDecoder();
}

jobject ImageDecoder::getPixels_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_ImageDecoder[0]);
}

jobject ImageDecoder::getPixels_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_ImageDecoder();
    return (jobject) env->CallObjectMethod(_this, methods_ImageDecoder[0]);
}

jobject ImageDecoder::decodeBitmapFromNativeUri_unsafe(jstring nativeUri) {
    CALL_JNIENV();
    initialize_ImageDecoder();
    return (jobject) env->CallStaticObjectMethod(class_ImageDecoder, methods_ImageDecoder[1], nativeUri);
}

jint ImageDecoder::getHeight() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_ImageDecoder[2]);
}

jint ImageDecoder::getHeight_(jobject _this) {
    CALL_JNIENV();
    initialize_ImageDecoder();
    return (jint) env->CallIntMethod(_this, methods_ImageDecoder[2]);
}

jobject ImageDecoder::decodeFromStream_unsafe(jobject stream) {
    CALL_JNIENV();
    initialize_ImageDecoder();
    return (jobject) env->CallStaticObjectMethod(class_ImageDecoder, methods_ImageDecoder[3], stream);
}

jobject ImageDecoder::decodeBitmapFromStream_unsafe(jobject stream) {
    CALL_JNIENV();
    initialize_ImageDecoder();
    return (jobject) env->CallStaticObjectMethod(class_ImageDecoder, methods_ImageDecoder[4], stream);
}

jint ImageDecoder::getWidth() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_ImageDecoder[5]);
}

jint ImageDecoder::getWidth_(jobject _this) {
    CALL_JNIENV();
    initialize_ImageDecoder();
    return (jint) env->CallIntMethod(_this, methods_ImageDecoder[5]);
}

jobject ImageDecoder::decodeFromBitmap_unsafe(jobject image, jboolean recycle) {
    CALL_JNIENV();
    initialize_ImageDecoder();
    return (jobject) env->CallStaticObjectMethod(class_ImageDecoder, methods_ImageDecoder[6], image, recycle);
}

jc_sp<ImageDecoder> ImageDecoder::wrap(jobject obj) {
    return jc_sp<ImageDecoder>( mark_new ImageDecoder(obj));
}

jc_sp<ImageDecoder> ImageDecoder::global(jobject obj) {
    return jc_sp<ImageDecoder>( (ImageDecoder*)(mark_new ImageDecoder(obj))->addGlobalRef());
}

jclass ImageDecoder::getClass() {
    initialize_ImageDecoder();
    return class_ImageDecoder;
}

}

