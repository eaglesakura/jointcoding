package com.eaglesakura.jc.android.stub;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;

@JCClass(
         aliasClassName = "java.lang.RuntimeException",
         cppNamespace = "ndk",
         className = "JniRuntimeException")
public class RuntimeExceptionStub extends ExceptionStub {

    static final long serialVersionUID = 0x01;

}
