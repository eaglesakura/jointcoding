/**
 * export from "Joint Coding Project"
 */
#ifndef __NATIVEKEYEVENT_H__
#define __NATIVEKEYEVENT_H__
#include "jointcoding-android.h"
#include "jcKeyEventProtocol.h"

namespace ndk {

class NativeKeyEvent: public ::ndk::JniWrapper, public KeyEventProtocol {
protected:
    NativeKeyEvent(jobject obj);
public:
    virtual ~NativeKeyEvent(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jint getEventType();
    static jint getEventType_(jobject _this);
    virtual jint getKeyCode();
    static jint getKeyCode_(jobject _this);

    static const ::jc::s32 EVENT_TYPE_DOWN;
    static const ::jc::s32 EVENT_TYPE_UP;
    static const ::jc::s32 KEYCODE_UNKNOWN;
    static const ::jc::s32 KEYCODE_UP;
    static const ::jc::s32 KEYCODE_DOWN;
    static const ::jc::s32 KEYCODE_LEFT;
    static const ::jc::s32 KEYCODE_RIGHT;
    static const ::jc::s32 KEYCODE_ENTER;
    static const ::jc::s32 KEYCODE_BACK;

    static jc_sp<NativeKeyEvent> wrap(jobject obj);
    static jc_sp<NativeKeyEvent> global(jobject obj);

    static jclass getClass();
};

}

#endif // __NATIVEKEYEVENT_H__
