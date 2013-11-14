/*
 * Mutex.cpp
 *
 *  Created on: 2012/07/21
 */
#include    "jointcoding.h"
#include    "jc/system/Macro.h"
#include    "jc/thread/Mutex.h"

#include    "prv_Thread.h"

namespace jc {

Mutex::Mutex() {
    mutex = ImplCapsule(new native_mutex(), JC_CAPSULE_RELEAE(native_mutex) );


    jcmarkvoid(this);
}

Mutex::~Mutex() {
    jcunmarkvoid(this);
}

}
