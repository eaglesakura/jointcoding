/**
 * export from "Joint Coding Project"
 */
#ifndef __NATIVETOUCHEVENT_H__
#define __NATIVETOUCHEVENT_H__
#include "jointcoding-android.h"
#include "jcTouchEventProtocol.h"

namespace ndk {

class NativeTouchEvent: public ::ndk::JniWrapper, public TouchEventProtocol {
protected:
    NativeTouchEvent(jobject obj);
public:
    virtual ~NativeTouchEvent(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jint getTouchID();
    static jint getTouchID_(jobject _this);
    virtual jfloat getEventPosX();
    static jfloat getEventPosX_(jobject _this);
    virtual jstring toString();
    static jstring toString_(jobject _this);
    virtual jint getEventType();
    static jint getEventType_(jobject _this);
    virtual jfloat getEventPosY();
    static jfloat getEventPosY_(jobject _this);

    static const ::jc::s32 EVENT_TYPE_BEGIN;
    static const ::jc::s32 EVENT_TYPE_MOVE;
    static const ::jc::s32 EVENT_TYPE_END;
    static const ::jc::s32 EVENT_TYPE_CANCEL;

    static jc_sp<NativeTouchEvent> wrap(jobject obj);
    static jc_sp<NativeTouchEvent> global(jobject obj);

    static jclass getClass();
};

}

#endif // __NATIVETOUCHEVENT_H__
