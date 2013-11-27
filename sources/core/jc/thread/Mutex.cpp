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
    mutex.reset(mark_new ::boost::recursive_mutex());
}

Mutex::~Mutex() {
    mutex.reset();
}

}
