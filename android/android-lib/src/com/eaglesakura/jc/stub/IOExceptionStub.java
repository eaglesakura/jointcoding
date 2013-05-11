package com.eaglesakura.jc.stub;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;

@JCClass(
         aliasClassName = "java.lang.IOException",
         cppNamespace = "ndk",
         className = "JniIOException")
public class IOExceptionStub extends ExceptionStub {

    static final long serialVersionUID = 0x01;

}
