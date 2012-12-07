package com.eaglesakura.jc.android.io.test;

import android.test.suitebuilder.annotation.SmallTest;

import com.eaglesakura.jc.android.testsupport.JniTestCase;

@SmallTest
public class UriTest extends JniTestCase {

    public void test_Uri_checmeTest() {
        callTestFunction("Uri_checmeTest");
    }

    public void test_Uri_filenameTest() {
        callTestFunction("Uri_filenameTest");
    }

}
