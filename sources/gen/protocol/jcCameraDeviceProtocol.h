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
    static const ::jc::s32 ORIENTATION_ROTATE_0;
    static const ::jc::s32 ORIENTATION_ROTATE_90;
    static const ::jc::s32 ORIENTATION_ROTATE_180;
    static const ::jc::s32 ORIENTATION_ROTATE_270;
    static const ::jc::s32 FOCUSMODE_NONE;
    static const ::jc::s32 FOCUSMODE_PROCESSING;
    static const ::jc::s32 FOCUSMODE_COMPLETED;
    static const ::jc::s32 FOCUSMODE_FAILED;
};

}

#endif // __CAMERADEVICEPROTOCOL_H__
