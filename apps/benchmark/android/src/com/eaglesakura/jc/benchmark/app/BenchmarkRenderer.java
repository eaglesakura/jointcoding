package com.eaglesakura.jc.benchmark.app;

import com.eaglesakura.jc.framework.app.JointApplicationRenderer;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "es")
public class BenchmarkRenderer extends JointApplicationRenderer {

    public BenchmarkRenderer() {
    }

    @JCMethod(
              nativeMethod = true)
    @Override
    protected native void createNativeContext();
}
