/*
 * CdtTestRunner.h
 *
 *  Created on: 2012/12/20
 */

#ifndef CDTTESTRUNNER_H_
#define CDTTESTRUNNER_H_

#include    "jointcoding.h"
#include    "jointcoding-testsupport.h"

namespace jc {

class CdtTestRunner {
public:
    /**
     * 初期化を行う
     */
    static void initialize(int argv, char *args[]);

    /**
     * テスト関数を実行する
     */
    static void addTestFunction(const charactor *function_name, const test_function_ptr function_ptr);

    /**
     * テストの実行を行う
     */
    static void run();
};

#define add_unittest(function_name)    void function_name();  ::jc::CdtTestRunner::addTestFunction(#function_name, (::jc::test_function_ptr)function_name);

}

#endif /* CDTTESTRUNNER_H_ */
