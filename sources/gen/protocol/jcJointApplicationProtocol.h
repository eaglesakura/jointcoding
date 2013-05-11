/**
 * export from "Joint Coding Project"
 */
#ifndef __JOINTAPPLICATIONPROTOCOL_H__
#define __JOINTAPPLICATIONPROTOCOL_H__
#include "jointcoding.h"
namespace jc {

class JointApplicationProtocol {
public:
    virtual ~JointApplicationProtocol(void){
    }


    static const ::jc::s32 State_Initializing;
    static const ::jc::s32 State_Running;
    static const ::jc::s32 State_Paused;
    static const ::jc::s32 State_Destroyed;
    static const ::jc::s32 QueryKey_ApplicationState;
    static const ::jc::s32 PostKey_SurfaceSize;
};

}

#endif // __JOINTAPPLICATIONPROTOCOL_H__
