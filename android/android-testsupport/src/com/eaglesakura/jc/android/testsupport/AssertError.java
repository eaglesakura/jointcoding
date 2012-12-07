package com.eaglesakura.jc.android.testsupport;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;

@JCClass(
         cppNamespace = "jc")
public class AssertError extends Error {
    /**
     * UID生成
     */
    static final long serialVersionUID = 1;

    public AssertError() {
    }

    public AssertError(String detail) {
        super(detail);
    }

    public AssertError(Throwable t) {
        super(t);
    }

    public AssertError(String detail, Throwable t) {
        super(detail, t);
    }
}
