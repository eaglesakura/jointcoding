/**
 * export from "Joint Coding Project"
 */
#ifndef __SURFACESPECPROTOCOL_H__
#define __SURFACESPECPROTOCOL_H__
#include "jointcoding.h"
namespace jc {

class SurfaceSpecProtocol {
public:
    virtual ~SurfaceSpecProtocol(void){
    }


    static const ::jc::charactor* KEY_PixelFormat;
    static const ::jc::charactor* KEY_HasDepth;
    static const ::jc::charactor* KEY_HasStencil;
    static const ::jc::charactor* KEY_AndroidTextureView;
    static const ::jc::charactor* KEY_AndroidSurfaceViewOnTop;
};

}

#endif // __SURFACESPECPROTOCOL_H__
