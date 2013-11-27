/*
 * MutexLock.cpp
 *
 *  Created on: 2012/07/21
 */
#include    "jointcoding.h"
#include    "jc/system/Macro.h"
#include    "jc/thread/Mutex.h"

#include    "prv_Thread.h"

using namespace boost;

namespace jc {

MutexLock::MutexLock(const jc::Mutex &mtx) {
    this->mutex = mtx.mutex;
    this->lock = (void*) mark_new native_lock(*this->mutex.get());
}

MutexLock::~MutexLock() {
    {
        native_lock *pLock = (native_lock*)this->lock;
        SAFE_DELETE(pLock);
        this->lock = NULL;
    }
    mutex.reset();
}

}
