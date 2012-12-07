/*
 * jcTestSupport.h
 *
 *  Created on: 2012/07/06
 */

#ifndef JCTESTSUPPORT_H_
#define JCTESTSUPPORT_H_

#include "jointcoding.h"

namespace jc {

/**
 * trueであることを宣言する
 */
void __assertTrue(const charactor* file, const s32 line, const bool is, bool *success, const charactor* message);

/**
 * 共通部分
 */
#define __JC_ASSERT(is, msg) { bool __success__ = false; __assertTrue(jc::__getFileName(__FILE__), __LINE__, is == true, &__success__, msg); if(!__success__){ return; } }

/**
 * trueであることを宣言する
 */
#define _assertTrue(is )    __JC_ASSERT(is == true, "( " #is " ) is false")

/**
 * falseであることを宣言する
 */
#define _assertFalse(is)    __JC_ASSERT(is == false, "( " #is " ) is true")

/**
 * 対象がNullでないことを宣言する
 */
#define _assertNotNull(ptr) __JC_ASSERT(ptr != NULL, "( " #ptr " ) is NULL")

/**
 * 対象がNullであることを宣言する
 */
#define _assertNull(ptr) __JC_ASSERT(ptr == NULL, "( " #ptr " ) is not NULL")

/**
 * 対象が同一であることを宣言する
 */
#define _assertEquals( a, b ) __JC_ASSERT(a == b, "( " #a " ) not equals ( " #b " )")

/**
 * 対象が同一でないことを宣言する
 */
#define _assertNotEquals(a, b ) __JC_ASSERT(a != b, "( " #a " ) equals ( " #b " )")

/**
 * テストが失敗したことを明示する
 */
#define _test_failed(message)   __JC_ASSERT(false, message)

#ifdef  BUILD_Android
#define TESTFUNCTION    __attribute__ ((visibility ("default")))
#else
#define TESTFUNCTION
#endif

/**
 * テスト呼び出し用のエントリポイント
 */
typedef void (*test_function_ptr)();

/**
 * テストをビルドする場合のみマクロが定義される
 * #ifdefを利用して切り分けること。
 */
#define TEST_BUILD_ENABLE 1
}
#endif /* JCTESTSUPPORT_H_ */
