/*
 * jcThreadImpl.h
 *
 *  Created on: 2012/09/28
 */

#ifndef PRV_JCTHREADIMPL_H_
#define PRV_JCTHREADIMPL_H_
#define JC_NATIVE_MUTEX
#define JC_MUTEXLOCK_TYPE

/**
 * boost側のmutexクラスへ変換する
 */
#define JC_TO_NATIVE_MUTEX(native_mutex)     (*((JC_NATIVE_MUTEX*)(native_mutex)))

/**
 * 実装部はboostに一任する
 */
#include    "boost/thread.hpp"

namespace jc {

/**
 * boost側のmutexクラス
 */
typedef ::boost::recursive_mutex native_mutex;

/**
 * boost側のlockクラス
 */
typedef ::boost::recursive_mutex::scoped_lock native_lock;
}

#endif /* JCTHREADIMPL_H_ */
