package com.eaglesakura.jc.android.stub;

import android.view.Surface;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         aliasClassName = "android.view.SurfaceHolder",
         cppNamespace = "ndk",
         className = "SurfaceHolder")
public class SurfaceHolderStub {

    @JCMethod
    public Surface getSurface() {
        return null;
    }

}
