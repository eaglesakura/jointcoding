package com.eaglesakura.jc.android.util.test;

import android.test.suitebuilder.annotation.SmallTest;

import com.eaglesakura.jc.android.testsupport.JniTestCase;

@SmallTest
public class ListTest extends JniTestCase {

    public void test_OrderAccessList_make() {
        callTestFunction("OrderAccessList_make");
    }

}
