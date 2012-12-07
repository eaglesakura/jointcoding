package com.eaglesakura.lib.jc.sample.jnimake;

import com.eaglesakura.lib.jc.annotation.jnimake.JCAliasMethod;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         className = "JointExportSample",
         cppNamespace = "jc",
         aliasClassName = "com.eaglesakura.export.test.SampleExportClass2")
public class SampleExportClass implements SampleInterface {

    @JCField
    public static final String CONST_MOJIRETU = "文字列";

    @JCMethod
    public String setHoge(int value, String str) {
        return null;
    }

    @JCMethod
    public Object obj(Object obj) {
        return null;
    }

    @JCMethod
    @Override
    public boolean equals(Object obj) {
        return super.equals(obj);
    }

    @JCMethod
    public void setHoge() {

    }

    @JCMethod
    public static void setHoge(int value) {

    }

    @JCMethod
    public void read(byte[] buffer, int index, int length) {

    }

    @JCMethod
    public SampleExportClass getHoge(int value) {
        return null;
    }

    @JCMethod
    public SampleExportClass doHoge(SampleExportClass clz) {
        return null;
    }

    @JCMethod
    public SampleExportClass[] doHogeArray(SampleExportClass[] clz) {
        return null;
    }

    @JCMethod
    public int[] doIntArray(int[] array) {
        return array;
    }

    @JCMethod
    public Object[] doObjectArray(Object[] array) {
        return array;
    }

    @JCMethod(
              nativeMethod = true)
    public native void nativeCall(int value, String str);

    @JCMethod
    @Override
    public void method(int num) {

    }

    @JCMethod(
              factory = "JCObject")
    public native int factory(int num, int vec);

    @JCAliasMethod(
                   className = "JCObject")
    native boolean equals2(int pointer, SampleExportClass obj);

    @JCAliasMethod(
                   className = "JCObject")
    native void hash(int pointer);
}
