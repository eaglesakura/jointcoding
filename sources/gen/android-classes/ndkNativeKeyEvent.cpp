/**
 * export from "Joint Coding Project"
 */
#include "ndkNativeKeyEvent.h"

namespace ndk {

const ::jc::charactor* NativeKeyEvent::CLASS_SIGNATURE = "com/eaglesakura/jc/android/ui/NativeKeyEvent";
const ::jc::s32 NativeKeyEvent::EVENT_TYPE_DOWN = 0;
const ::jc::s32 NativeKeyEvent::EVENT_TYPE_UP = 1;
const ::jc::s32 NativeKeyEvent::KEYCODE_UNKNOWN = -1;
const ::jc::s32 NativeKeyEvent::KEYCODE_UP = 0;
const ::jc::s32 NativeKeyEvent::KEYCODE_DOWN = 1;
const ::jc::s32 NativeKeyEvent::KEYCODE_LEFT = 2;
const ::jc::s32 NativeKeyEvent::KEYCODE_RIGHT = 3;
const ::jc::s32 NativeKeyEvent::KEYCODE_ENTER = 4;
const ::jc::s32 NativeKeyEvent::KEYCODE_BACK = 5;

static jclass class_NativeKeyEvent = NULL;

#define methods_NativeKeyEvent_LENGTH 2

#if methods_NativeKeyEvent_LENGTH
static jmethodID methods_NativeKeyEvent[2];
#endif

static void initialize_NativeKeyEvent() {
    // loaded !
    if (class_NativeKeyEvent) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_NativeKeyEvent = env->FindClass(NativeKeyEvent::CLASS_SIGNATURE);
    class_NativeKeyEvent = (jclass)::ndk::change_globalref(env, class_NativeKeyEvent);
    

    // load methods
    {
        methods_NativeKeyEvent[0] = ::ndk::JniWrapper::loadMethod(class_NativeKeyEvent, "getEventType", "()I", false);
        methods_NativeKeyEvent[1] = ::ndk::JniWrapper::loadMethod(class_NativeKeyEvent, "getKeyCode", "()I", false);

    }
}

NativeKeyEvent::NativeKeyEvent(jobject obj): ::ndk::JniWrapper(obj){
    initialize_NativeKeyEvent();
}

jint NativeKeyEvent::getEventType() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_NativeKeyEvent[0]);
}

jint NativeKeyEvent::getEventType_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeKeyEvent();
    return (jint) env->CallIntMethod(_this, methods_NativeKeyEvent[0]);
}

jint NativeKeyEvent::getKeyCode() {
    CALL_JNIENV();
    return (jint) env->CallIntMethod(this->getObject(), methods_NativeKeyEvent[1]);
}

jint NativeKeyEvent::getKeyCode_(jobject _this) {
    CALL_JNIENV();
    initialize_NativeKeyEvent();
    return (jint) env->CallIntMethod(_this, methods_NativeKeyEvent[1]);
}

jc_sp<NativeKeyEvent> NativeKeyEvent::wrap(jobject obj) {
    return jc_sp<NativeKeyEvent>( new NativeKeyEvent(obj));
}

jc_sp<NativeKeyEvent> NativeKeyEvent::global(jobject obj) {
    return jc_sp<NativeKeyEvent>( (NativeKeyEvent*)(new NativeKeyEvent(obj))->addGlobalRef());
}

jclass NativeKeyEvent::getClass() {
    initialize_NativeKeyEvent();
    return class_NativeKeyEvent;
}

}

