/*
 * TestRunner.cpp
 *
 *  Created on: 2012/12/20
 */

#include    "jointcoding.h"
#include    "jointcoding-testsupport.h"
#include    "jctest/CdtTestRunner.h"

#include    <vector>

namespace jc {

struct TestContext {
    /**
     * 関数ポインタ
     */
    test_function_ptr function;

    /**
     * 関数名
     */
    String name;

    /**
     * 成功フラグ
     */
    jcboolean success;
};

/**
 * 実行対象のテスト一覧
 */
static std::vector<TestContext> tests;

/**
 * 現在実行中のテスト番号
 */
static s32 current_test = 0;

/**
 * 成功数
 */
static s32 successNum = 0;

/**
 * 失敗数
 */
static s32 failedNum = 0;

/**
 * trueであることを宣言する
 */
void __assertTrue(const charactor* file, const s32 line, const bool is, bool *success, const charactor* message) {
    if (is) {
        (*success) = true;
        return;
    }

    tests[current_test].success = jcfalse;
    (*success) = false;

    // エラー出力を行う
    fprintf(stderr, "ASSERT ERROR(%s | L %d) | %s", file, line, message);
}

/**
 * 初期化を行う
 */
void CdtTestRunner::initialize(int argv, char *args[]) {

    {
        jclogf("args(%d)", argv);
        for (int i = 0; i < argv; ++i) {
            jclogf("arg[i] = %s", i, args[i]);
        }
    }

    tests.clear();
    current_test = 0;
    successNum = 0;
    failedNum = 0;
}

/**
 * テスト関数を実行する
 */
void CdtTestRunner::addTestFunction(const charactor *function_name, const test_function_ptr function_ptr) {
    TestContext context;
    context.function = function_ptr;
    context.name = function_name;
    context.success = jctrue;

    tests.push_back(context);
}

/**
 * テストの実行を行う
 */
void CdtTestRunner::run() {

    for (int i = 0; i < (int) tests.size(); ++i) {
        TestContext &context = tests[i];

        jclogf("----------------    %s    ----------------", context.name.c_str());
        // テストを実行
        try {
            context.function();
        } catch (Exception &e) {
            jcloge(e);
            context.success = jcfalse;
        }
        if (context.success) {
            jclogf("Test Success( %s )", context.name.c_str());
            ++successNum;
        } else {
            jclogf("Test Failed( %s )", context.name.c_str());
            ++failedNum;
        }

        ++current_test;
    }

    jclog("----------------    Test Finished!!    ----------------");
    jclogf("Test Success( %d )", successNum);
    jclogf("Test Failed ( %d )", failedNum);
    jclog("-------------------------------------------------------");
}

}
