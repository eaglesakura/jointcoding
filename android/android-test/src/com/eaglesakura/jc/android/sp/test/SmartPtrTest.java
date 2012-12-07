package com.eaglesakura.jc.android.sp.test;

import android.test.suitebuilder.annotation.SmallTest;

import com.eaglesakura.jc.android.testsupport.JniTestCase;

@SmallTest
public class SmartPtrTest extends JniTestCase {

    public void test_SmartPtr_ref_Test() {
        callTestFunction("SmartPtr_ref_Test");
    }

    public void test_SmartPtr_set_Test() {
        callTestFunction("SmartPtr_set_Test");
    }

    public void test_SmartPtr_boost_set_Test() {
        callTestFunction("SmartPtr_boost_set_Test");
    }

    public void test_SmartPtr_boost_voidPtr_Test() {
        callTestFunction("SmartPtr_boost_voidPtr_Test");
    }

    public void test_SmartPtr_boost_voidProtocol_Test() {
        callTestFunction("SmartPtr_boost_voidProtocol_Test");
    }

    public void test_SmartPtr_boost_voidProtocol_set_Test() {
        callTestFunction("SmartPtr_boost_voidProtocol_set_Test");
    }
}
