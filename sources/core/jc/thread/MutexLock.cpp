/*
 * MutexLock.cpp
 *
 *  Created on: 2012/07/21
 */
#include    "jc/system/Macro.h"
#include    "jc/thread/Mutex.h"

#include    "prv_Thread.h"

using namespace boost;

namespace jc {

MutexLock::MutexLock(const jc::Mutex &mtx) {
    this->mutex = mtx.mutex;
    nativeLock = (new native_lock(mtx.mutex.as<native_mutex>()));
}

MutexLock::~MutexLock() {
    IMPL_SAFE_DELETE(nativeLock, native_lock);
}

}
