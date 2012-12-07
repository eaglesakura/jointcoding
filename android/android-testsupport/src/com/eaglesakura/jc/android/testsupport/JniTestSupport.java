package com.eaglesakura.jc.android.testsupport;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * テストサポートクラス
 *
 */
@JCClass(
         cppNamespace = "jc")
public class JniTestSupport {
    static {
    }

    /**
     * JNIを通してテストを行う。
     * @param packageName
     * @param libName
     * @param functionName
     */
    @JCMethod(
              nativeMethod = true)
    static native void callJniTest(final String packageName, final String libName, final String functionName);
}
