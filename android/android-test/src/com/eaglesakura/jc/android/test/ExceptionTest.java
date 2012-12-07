package com.eaglesakura.jc.android.test;

import android.test.suitebuilder.annotation.SmallTest;

import com.eaglesakura.jc.android.testsupport.JniTestCase;

@SmallTest
public class ExceptionTest extends JniTestCase {

    public void test_Exception_throwTest() {
        callTestFunction("Exception_throwTest");
    }
}
