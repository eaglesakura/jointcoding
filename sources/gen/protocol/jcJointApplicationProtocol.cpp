/**
 * export from "Joint Coding Project"
 */
#include "jcJointApplicationProtocol.h"

namespace jc {

const ::jc::s32 JointApplicationProtocol::State_Initializing = 0;
const ::jc::s32 JointApplicationProtocol::State_Running = 1;
const ::jc::s32 JointApplicationProtocol::State_Paused = 2;
const ::jc::s32 JointApplicationProtocol::State_Destroyed = 3;
const ::jc::s32 JointApplicationProtocol::PostKey_QueryApplicationState = 1048576;
const ::jc::s32 JointApplicationProtocol::PostKey_RequestSurfaceSpecs = 1048577;
const ::jc::s32 JointApplicationProtocol::PostKey_SurfaceSize = 1048578;
const ::jc::s32 JointApplicationProtocol::PostKey_StateRequest = 1048579;
const ::jc::s32 JointApplicationProtocol::SystemTask_Mainloop = 16777216;


}

