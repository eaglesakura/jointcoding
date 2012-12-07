package com.eaglesakura.jc.android.testsupport;

import android.test.AndroidTestCase;

import com.eaglesakura.jc.android.app.NativeContext;

/**
 * 
 *
 */
public abstract class JniTestCase extends AndroidTestCase {
    /**
     * テスト対象のsoファイル名を取得する
     * @return
     */
    protected final String getTargetLibName() {
        return "jointcoding-apptest";
    }

    /**
     * テスト対象のライブラリ名
     */
    String libName;

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        // default libs
        {
            System.loadLibrary("jointcoding-lib");
            System.loadLibrary("jointcoding-testsupport");
        }
        libName = getTargetLibName();
        System.loadLibrary(libName);
        NativeContext.getInstance(getContext());
    }

    /**
     * JNIテストを呼び出す
     * @param functionName
     */
    protected void callTestFunction(String functionName) {
        JniTestSupport.callJniTest(getContext().getPackageName(), libName, functionName);
    }

}
