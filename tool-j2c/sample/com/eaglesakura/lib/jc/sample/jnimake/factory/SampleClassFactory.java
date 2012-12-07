package com.eaglesakura.lib.jc.sample.jnimake.factory;

import com.eaglesakura.lib.jc.annotation.nativefactory.JCFactoryClass;
import com.eaglesakura.lib.jc.annotation.nativefactory.JCNativeFactoryMethod;

@JCFactoryClass
public class SampleClassFactory {
    /**
     * JCObject()を作成する
     * @return
     */
    @JCNativeFactoryMethod(
                           className = "JCObject")
    public native static int makeJCObject();
}
