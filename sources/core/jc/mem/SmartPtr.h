/*
 * SmartPtr.h
 *
 *  Created on: 2012/05/13
 *      Author: Takeshi
 */

#ifndef JCSMARTPTR_H_
#define JCSMARTPTR_H_

#include    "boost/smart_ptr/shared_ptr.hpp"
#include    "boost/smart_ptr/weak_ptr.hpp"
#include    "boost/smart_ptr/shared_array.hpp"
#include    "boost/smart_ptr/intrusive_ptr.hpp"

/**
 * 共有ポインタ
 */
#define jc_sp ::boost::shared_ptr

/**
 * スマートポインタのdynamic_castを行う
 */
#define jc_dynamic_cast     ::boost::dynamic_pointer_cast

/**
 * 共有配列
 */
#define jc_sa ::boost::shared_array
/**
 * 弱参照
 */
#define jc_wp ::boost::weak_ptr


#endif /* SMARTPTR_H_ */
