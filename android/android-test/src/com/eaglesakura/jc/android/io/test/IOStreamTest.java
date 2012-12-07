package com.eaglesakura.jc.android.io.test;

import android.test.suitebuilder.annotation.SmallTest;

import com.eaglesakura.jc.android.testsupport.JniTestCase;

@SmallTest
public class IOStreamTest extends JniTestCase {

    public void test_Inputstream_openTest() {
        callTestFunction("Inputstream_openTest");
    }

    public void test_Inputstream_openJpegTest() {
        callTestFunction("Inputstream_openJpegTest");
    }

    public void test_Outputstream_memwriteTest() {
        callTestFunction("Outputstream_memwriteTest");
    }
}
