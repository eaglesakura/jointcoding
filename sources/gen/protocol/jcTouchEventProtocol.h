/**
 * export from "Joint Coding Project"
 */
#ifndef __TOUCHEVENTPROTOCOL_H__
#define __TOUCHEVENTPROTOCOL_H__
#include "jointcoding.h"
namespace jc {

class TouchEventProtocol {
public:
    virtual ~TouchEventProtocol(void){
    }

    virtual ::jc::s32 getTouchID() = 0;
    virtual float getEventPosX() = 0;
    virtual ::jc::s32 getEventType() = 0;
    virtual float getEventPosY() = 0;

    static const ::jc::s32 EVENT_TYPE_BEGIN;
    static const ::jc::s32 EVENT_TYPE_MOVE;
    static const ::jc::s32 EVENT_TYPE_END;
    static const ::jc::s32 EVENT_TYPE_CANCEL;
};

}

#endif // __TOUCHEVENTPROTOCOL_H__
