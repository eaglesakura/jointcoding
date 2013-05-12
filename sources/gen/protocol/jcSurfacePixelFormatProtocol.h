/**
 * export from "Joint Coding Project"
 */
#ifndef __SURFACEPIXELFORMATPROTOCOL_H__
#define __SURFACEPIXELFORMATPROTOCOL_H__
#include "jointcoding.h"
namespace jc {

class SurfacePixelFormatProtocol {
public:
    virtual ~SurfacePixelFormatProtocol(void){
    }


    static const ::jc::s32 RGB8;
    static const ::jc::s32 RGBA8;
};

}

#endif // __SURFACEPIXELFORMATPROTOCOL_H__
