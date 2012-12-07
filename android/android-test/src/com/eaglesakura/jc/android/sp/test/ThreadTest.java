package com.eaglesakura.jc.android.sp.test;

import android.test.suitebuilder.annotation.LargeTest;

import com.eaglesakura.jc.android.testsupport.JniTestCase;

@LargeTest
public class ThreadTest extends JniTestCase {

    /**
     * Looper生成テスト
     */
    @LargeTest
    public void test_Thread_looperTest() {
        callTestFunction("Thread_looperTest");
    }

    /**
     * Boost版Threadの生成テスト
     */
    @LargeTest
    private void test_Thread_boost_createTest() {
        callTestFunction("Thread_boost_createTest");
    }

    /**
     * Boost版スレッドの自動リリーステスト
     */
    @LargeTest
    private void test_Thread_boost_autoreleaseTest() {
        callTestFunction("Thread_boost_autoreleaseTest");
    }

    public void test_Thread_createTest() {
        callTestFunction("Thread_createTest");
    }

    public void test_Thread_autoreleaseTest() {
        callTestFunction("Thread_autoreleaseTest");
    }

    public void test_Thread_otherThreadJniCallTest() {
        callTestFunction("Thread_otherThreadJniCallTest");
    }
}
