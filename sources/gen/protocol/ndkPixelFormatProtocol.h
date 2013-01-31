/**
 * export from "Joint Coding Project"
 */
#ifndef __PIXELFORMATPROTOCOL_H__
#define __PIXELFORMATPROTOCOL_H__
#include "jointcoding.h"
namespace ndk {

class PixelFormatProtocol {
public:
    virtual ~PixelFormatProtocol(void){
    }


    static const ::jc::s32 ANDROID_PIXELFORMAT_RGB565;
    static const ::jc::s32 ANDROID_PIXELFORMAT_RGB888;
    static const ::jc::s32 ANDROID_PIXELFORMAT_RGBA8888;
};

}

#endif // __PIXELFORMATPROTOCOL_H__
