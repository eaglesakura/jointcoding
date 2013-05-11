/**
 * export from "Joint Coding Project"
 */
#include "jcJointApplicationProtocol.h"

namespace jc {

const ::jc::s32 JointApplicationProtocol::State_Initializing = 0;
const ::jc::s32 JointApplicationProtocol::State_Running = 1;
const ::jc::s32 JointApplicationProtocol::State_Paused = 2;
const ::jc::s32 JointApplicationProtocol::State_Destroyed = 3;
const ::jc::s32 JointApplicationProtocol::QueryKey_ApplicationState = 65536;
const ::jc::s32 JointApplicationProtocol::PostKey_SurfaceSize = 65537;


}

