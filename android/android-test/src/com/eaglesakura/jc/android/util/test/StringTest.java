package com.eaglesakura.jc.android.util.test;

import android.test.suitebuilder.annotation.SmallTest;

import com.eaglesakura.jc.android.testsupport.JniTestCase;

@SmallTest
public class StringTest extends JniTestCase {

    public void test_String_helloTest() {
        callTestFunction("String_helloTest");
    }
}
