/**
 * export from "Joint Coding Project"
 */
#include "ndkCameraWrapper.h"

namespace ndk {

const ::jc::charactor* CameraWrapper::CLASS_SIGNATURE = "com/eaglesakura/jc/hardware/camera/CameraWrapper";

static jclass class_CameraWrapper = NULL;

#define methods_CameraWrapper_LENGTH 16

#if methods_CameraWrapper_LENGTH
static jmethodID methods_CameraWrapper[16];
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
        methods_CameraWrapper[0] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "isAspectFrip", "()Z", false);
        methods_CameraWrapper[1] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "getPreviewHeight", "()I", false);
        methods_CameraWrapper[2] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "requestOrientation", "(I)Z", false);
        methods_CameraWrapper[3] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "cancelAutofocus", "()Z", false);
        methods_CameraWrapper[4] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "renderingToTexture", "()Z", false);
        methods_CameraWrapper[5] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "startAutofocus", "()Z", false);
        methods_CameraWrapper[6] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "startPreview", "(I)Z", false);
        methods_CameraWrapper[7] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "dispose", "()V", false);
        methods_CameraWrapper[8] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "getOrientationType", "()I", false);
        methods_CameraWrapper[9] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "getPreviewWidth", "()I", false);
        methods_CameraWrapper[10] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "popFocusMode", "()I", false);
        methods_CameraWrapper[11] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "createInstance", "(I)Lcom/eaglesakura/jc/hardware/camera/CameraWrapper;", true);
        methods_CameraWrapper[12] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "requestPreviewSize", "(IIII)V", false);
        methods_CameraWrapper[13] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "stopPreview", "()Z", false);
        methods_CameraWrapper[14] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "getTextureMatrix", "()Ljava/nio/FloatBuffer;", false);
        methods_CameraWrapper[15] = ::ndk::JniWrapper::loadMethod(class_CameraWrapper, "isAutofocusProcessing", "()Z", false);

    }
}

CameraWrapper::CameraWrapper(jobject obj): ::ndk::JniWrapper(obj){
    initialize_CameraWrapper();
}

jboolean CameraWrapper::isAspectFrip() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[0]);
}

jboolean CameraWrapper::isAspectFrip_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[0]);
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

jboolean CameraWrapper::requestOrientation(jint CameraDeviceProtocol_ORIENTATION) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[2], CameraDeviceProtocol_ORIENTATION);
}

jboolean CameraWrapper::requestOrientation_(jobject _this, jint CameraDeviceProtocol_ORIENTATION) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[2], CameraDeviceProtocol_ORIENTATION);
}

jboolean CameraWrapper::cancelAutofocus() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[3]);
}

jboolean CameraWrapper::cancelAutofocus_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[3]);
}

jboolean CameraWrapper::renderingToTexture() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[4]);
}

jboolean CameraWrapper::renderingToTexture_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[4]);
}

jboolean CameraWrapper::startAutofocus() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[5]);
}

jboolean CameraWrapper::startAutofocus_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[5]);
}

jboolean CameraWrapper::startPreview(jint textureName) {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[6], textureName);
}

jboolean CameraWrapper::startPreview_(jobject _this, jint textureName) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[6], textureName);
}

void CameraWrapper::dispose() {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_CameraWrapper[7]);
}

void CameraWrapper::dispose_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    env->CallVoidMethod(_this, methods_CameraWrapper[7]);
}

jint CameraWrapper::getOrientationType() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_CameraWrapper[8]);
}

jint CameraWrapper::getOrientationType_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jint) env->CallIntMethod(_this, methods_CameraWrapper[8]);
}

jint CameraWrapper::getPreviewWidth() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_CameraWrapper[9]);
}

jint CameraWrapper::getPreviewWidth_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jint) env->CallIntMethod(_this, methods_CameraWrapper[9]);
}

jint CameraWrapper::popFocusMode() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_CameraWrapper[10]);
}

jint CameraWrapper::popFocusMode_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jint) env->CallIntMethod(_this, methods_CameraWrapper[10]);
}

jobject CameraWrapper::createInstance_unsafe(jint CameraDeviceProtocol_TYPE) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jobject) env->CallStaticObjectMethod(class_CameraWrapper, methods_CameraWrapper[11], CameraDeviceProtocol_TYPE);
}

void CameraWrapper::requestPreviewSize(jint width, jint height, jint minWidth, jint minHeight) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_CameraWrapper[12], width, height, minWidth, minHeight);
}

void CameraWrapper::requestPreviewSize_(jobject _this, jint width, jint height, jint minWidth, jint minHeight) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    env->CallVoidMethod(_this, methods_CameraWrapper[12], width, height, minWidth, minHeight);
}

jboolean CameraWrapper::stopPreview() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[13]);
}

jboolean CameraWrapper::stopPreview_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[13]);
}

jobject CameraWrapper::getTextureMatrix_unsafe() {
    CALL_JNIENV();
    return (jobject) env->CallObjectMethod(this->getObject(), methods_CameraWrapper[14]);
}

jobject CameraWrapper::getTextureMatrix_unsafe_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jobject) env->CallObjectMethod(_this, methods_CameraWrapper[14]);
}

jboolean CameraWrapper::isAutofocusProcessing() {
    CALL_JNIENV();
    return (jboolean) env->CallBooleanMethod(this->getObject(), methods_CameraWrapper[15]);
}

jboolean CameraWrapper::isAutofocusProcessing_(jobject _this) {
    CALL_JNIENV();
    initialize_CameraWrapper();
    return (jboolean) env->CallBooleanMethod(_this, methods_CameraWrapper[15]);
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

