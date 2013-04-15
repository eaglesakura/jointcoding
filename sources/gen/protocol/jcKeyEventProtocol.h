/**
 * export from "Joint Coding Project"
 */
#ifndef __KEYEVENTPROTOCOL_H__
#define __KEYEVENTPROTOCOL_H__
#include "jointcoding.h"
namespace jc {

class KeyEventProtocol {
public:
    virtual ~KeyEventProtocol(void){
    }

    virtual ::jc::s32 getEventType() = 0;
    virtual ::jc::s32 getKeyCode() = 0;

    static const ::jc::s32 EVENT_TYPE_DOWN;
    static const ::jc::s32 EVENT_TYPE_UP;
    static const ::jc::s32 KEYCODE_UNKNOWN;
    static const ::jc::s32 KEYCODE_UP;
    static const ::jc::s32 KEYCODE_DOWN;
    static const ::jc::s32 KEYCODE_LEFT;
    static const ::jc::s32 KEYCODE_RIGHT;
    static const ::jc::s32 KEYCODE_ENTER;
    static const ::jc::s32 KEYCODE_BACK;
};

}

#endif // __KEYEVENTPROTOCOL_H__
