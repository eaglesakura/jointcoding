/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeTouchEvent.h"

namespace ndk {

const ::jc::charactor* NativeTouchEvent::CLASS_SIGNATURE = "com/eaglesakura/jc/ui/NativeTouchEvent";
const ::jc::s32 NativeTouchEvent::EVENT_TYPE_BEGIN = 0;
const ::jc::s32 NativeTouchEvent::EVENT_TYPE_MOVE = 1;
const ::jc::s32 NativeTouchEvent::EVENT_TYPE_END = 2;
const ::jc::s32 NativeTouchEvent::EVENT_TYPE_CANCEL = 3;

static jclass class_NativeTouchEvent = NULL;

#define methods_NativeTouchEvent_LENGTH 5

#if methods_NativeTouchEvent_LENGTH
static jmethodID methods_NativeTouchEvent[5];
#endif

static void initialize_NativeTouchEvent() {
    // loaded !
    if (class_NativeTouchEvent) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeTouchEvent = env->FindClass(NativeTouchEvent::CLASS_SIGNATURE);
    class_NativeTouchEvent = (jclass)::ndk::change_globalref(env, class_NativeTouchEvent);
    

    // load methods
    {
        methods_NativeTouchEvent[0] = ::ndk::JniWrapper::loadMethod(class_NativeTouchEvent, "getTouchID", "()I", false);
        methods_NativeTouchEvent[1] = ::ndk::JniWrapper::loadMethod(class_NativeTouchEvent, "getEventPosX", "()F", false);
        methods_NativeTouchEvent[2] = ::ndk::JniWrapper::loadMethod(class_NativeTouchEvent, "toString", "()Ljava/lang/String;", false);
        methods_NativeTouchEvent[3] = ::ndk::JniWrapper::loadMethod(class_NativeTouchEvent, "getEventType", "()I", false);
        methods_NativeTouchEvent[4] = ::ndk::JniWrapper::loadMethod(class_NativeTouchEvent, "getEventPosY", "()F", false);

    }
}

NativeTouchEvent::NativeTouchEvent(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeTouchEvent();
}

jint NativeTouchEvent::getTouchID() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_NativeTouchEvent[0]);
}

jint NativeTouchEvent::getTouchID_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeTouchEvent();
    return (jint) env->CallIntMethod(_this, methods_NativeTouchEvent[0]);
}

jfloat NativeTouchEvent::getEventPosX() {
    CALL_JNIENV();
    return (jfloat) env->CallFloatMethod(this->getObject(), methods_NativeTouchEvent[1]);
}

jfloat NativeTouchEvent::getEventPosX_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeTouchEvent();
    return (jfloat) env->CallFloatMethod(_this, methods_NativeTouchEvent[1]);
}

jstring NativeTouchEvent::toString() {
    CALL_JNIENV();
    return (jstring) env->CallObjectMethod(this->getObject(), methods_NativeTouchEvent[2]);
}

jstring NativeTouchEvent::toString_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeTouchEvent();
    return (jstring) env->CallObjectMethod(_this, methods_NativeTouchEvent[2]);
}

jint NativeTouchEvent::getEventType() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_NativeTouchEvent[3]);
}

jint NativeTouchEvent::getEventType_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeTouchEvent();
    return (jint) env->CallIntMethod(_this, methods_NativeTouchEvent[3]);
}

jfloat NativeTouchEvent::getEventPosY() {
    CALL_JNIENV();
    return (jfloat) env->CallFloatMethod(this->getObject(), methods_NativeTouchEvent[4]);
}

jfloat NativeTouchEvent::getEventPosY_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeTouchEvent();
    return (jfloat) env->CallFloatMethod(_this, methods_NativeTouchEvent[4]);
}

jc_sp<NativeTouchEvent> NativeTouchEvent::wrap(jobject obj) {
    return jc_sp<NativeTouchEvent>( new NativeTouchEvent(obj));
}

jc_sp<NativeTouchEvent> NativeTouchEvent::global(jobject obj) {
    return jc_sp<NativeTouchEvent>( (NativeTouchEvent*)(new NativeTouchEvent(obj))->addGlobalRef());
}

jclass NativeTouchEvent::getClass() {
    initialize_NativeTouchEvent();
    return class_NativeTouchEvent;
}

}

