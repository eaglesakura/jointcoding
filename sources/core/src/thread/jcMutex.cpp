/*
 * Mutex.cpp
 *
 *  Created on: 2012/07/21
 */
#include    "jcMacro.h"
#include    "jcMutex.h"

#include    "prv_jcThread.h"

namespace jc {

Mutex::Mutex() {
    mutex = ImplCapsule(new native_mutex(), JC_CAPSULE_RELEAE(native_mutex) );
}

Mutex::~Mutex() {
}

}
