/**
 * export from "Joint Coding Project"
 */
#ifndef __CAMERADEVICEPROTOCOL_H__
#define __CAMERADEVICEPROTOCOL_H__
#include "jointcoding.h"
namespace jc {

class CameraDeviceProtocol {
public:
    virtual ~CameraDeviceProtocol(void){
    }


    static const ::jc::s32 TYPE_MAIN;
    static const ::jc::s32 TYPE_FRONT;
};

}

#endif // __CAMERADEVICEPROTOCOL_H__
