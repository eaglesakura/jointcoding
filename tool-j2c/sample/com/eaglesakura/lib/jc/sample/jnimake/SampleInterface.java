package com.eaglesakura.lib.jc.sample.jnimake;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;

@JCClass(
         mode = Mode.Protocol)
public interface SampleInterface {

    /**
     * 文字列
     */
    @JCField
    static final String CONST_STRING = "this is const string";

    /**
     * 整数
     */
    @JCField
    static final byte CONST_BYTE = 12;

    /**
     * 整数
     */
    @JCField
    static final short CONST_SHORT = 123;

    /**
     * 整数
     */
    @JCField
    static final int CONST_INT = 1234;

    /**
     * 整数
     */
    @JCField
    static final long CONST_LONG = 12345;

    /**
     * 浮動小数
     */
    @JCField
    static final float CONST_FLOAT = 1.23f;

    /**
     * 倍精度実数
     */
    @JCField
    static final double CONST_DOUBLE = 2.345;

    /**
     * 共通で定義するメソッド
     * @param num
     */
    @JCMethod
    void method(int num);
}
