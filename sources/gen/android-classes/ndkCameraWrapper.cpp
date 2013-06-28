/**
 * export from "Joint Coding Project"
 */
#include "ndkCameraWrapper.h"

namespace ndk {

const ::jc::charactor* CameraWrapper::CLASS_SIGNATURE = "com/eaglesakura/jc/hardware/camera/CameraWrapper";

static jclass class_CameraWrapper = NULL;

#define methods_CameraWrapper_LENGTH 8

#if methods_CameraWrapper_LENGTH
static jmethodID methods_CameraWrapper[8];
#endif

static void initialize_CameraWrapper() {
    // loaded !
    if (class_CameraWrapper) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_CameraWrapper = env->FindClass(CameraWrapper::CLASS_SIGNATURE);
    class_CameraWrapper = (jclass)::ndk::change_globalref(env, class_CameraWrapper);
    

    // load methods
    {
        methods_CameraWrapper[0] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "startPreview", "(I)Z", false);
        methods_CameraWrapper[1] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "getPreviewHeight", "()I", false);
        methods_CameraWrapper[2] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "dispose", "()V", false);
        methods_CameraWrapper[3] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "getPreviewWidth", "()I", false);
        methods_CameraWrapper[4] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "createInstance", "(I)Lcom/eaglesakura/jc/hardware/camera/CameraWrapper;", true);
        methods_CameraWrapper[5] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "renderingToTexture", "()Z", false);
        methods_CameraWrapper[6] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "stopPreview", "()Z", false);
        methods_CameraWrapper[7] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "getTextureMatrix", "()Ljava/nio/FloatBuffer;", false);

    }
}

CameraWrapper::CameraWrapper(jobject obj): ::ndk::JniWrapper(obj){
    initialize_CameraWrapper();
}

jboolean CameraWrapper::startPreview(jint textureName) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[0], textureName);
}

jboolean CameraWrapper::startPreview_(jobject _this, jint textureName) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[0], textureName);
}

jint CameraWrapper::getPreviewHeight() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_CameraWrapper[1]);
}

jint CameraWrapper::getPreviewHeight_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jint) env->CallIntMethod(_this, methods_CameraWrapper[1]);
}

void CameraWrapper::dispose() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_CameraWrapper[2]);
}

void CameraWrapper::dispose_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    env->CallVoidMethod(_this, methods_CameraWrapper[2]);
}

jint CameraWrapper::getPreviewWidth() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_CameraWrapper[3]);
}

jint CameraWrapper::getPreviewWidth_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jint) env->CallIntMethod(_this, methods_CameraWrapper[3]);
}

jobject CameraWrapper::createInstance_unsafe(jint CameraDeviceProtocol_TYPE) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jobject) env->CallStaticObjectMethod(class_CameraWrapper, methods_CameraWrapper[4], CameraDeviceProtocol_TYPE);
}

jboolean CameraWrapper::renderingToTexture() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[5]);
}

jboolean CameraWrapper::renderingToTexture_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[5]);
}

jboolean CameraWrapper::stopPreview() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[6]);
}

jboolean CameraWrapper::stopPreview_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[6]);
}

jobject CameraWrapper::getTextureMatrix_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_CameraWrapper[7]);
}

jobject CameraWrapper::getTextureMatrix_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jobject) env->CallObjectMethod(_this, methods_CameraWrapper[7]);
}

jc_sp<CameraWrapper> CameraWrapper::wrap(jobject obj) {
    return jc_sp<CameraWrapper>( new CameraWrapper(obj));
}

jc_sp<CameraWrapper> CameraWrapper::global(jobject obj) {
    return jc_sp<CameraWrapper>( (CameraWrapper*)(new CameraWrapper(obj))->addGlobalRef());
}

jclass CameraWrapper::getClass() {
    initialize_CameraWrapper();
    return class_CameraWrapper;
}

}

