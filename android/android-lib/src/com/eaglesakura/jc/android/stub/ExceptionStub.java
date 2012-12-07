package com.eaglesakura.jc.android.stub;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         aliasClassName = "java.lang.Exception",
         cppNamespace = "ndk",
         className = "JniException")
public class ExceptionStub extends Exception {

    static final long serialVersionUID = 0x01;

    @JCMethod
    @Override
    public void printStackTrace() {
        super.printStackTrace();
    }

    @JCMethod
    @Override
    public String getMessage() {
        return super.getMessage();
    }
}
