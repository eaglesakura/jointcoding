/*
 * MutexLock.cpp
 *
 *  Created on: 2012/07/21
 */
#include    "jcMacro.h"
#include    "jcMutex.h"

#include    "prv_jcThread.h"

using namespace boost;

namespace jc {

MutexLock::MutexLock(jc::Mutex &mtx) {
    this->mutex = mtx.mutex;
    nativeLock = (new native_lock(mtx.mutex.as<native_mutex>()));
}

MutexLock::~MutexLock() {
    IMPL_SAFE_DELETE(nativeLock, native_lock);
}

}
