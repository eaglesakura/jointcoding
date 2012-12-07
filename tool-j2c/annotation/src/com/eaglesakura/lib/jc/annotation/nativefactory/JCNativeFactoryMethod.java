package com.eaglesakura.lib.jc.annotation.nativefactory;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Nativeのクラスを吐き出すためのメソッドを示す。
 * 自動的にファクトリクラスが作成される。
 * 引数に利用できるのはプリミティブ型のみ
 * 
 * 制限
 * * 戻り値は必ずintであり、中身はポインタになっている。
 * * メソッドはnativeメソッドでなければならない。
 * 
 *
 */
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.CLASS)
public @interface JCNativeFactoryMethod {
    /**
     * 生成するクラス名
     * "JCObject"のように指定する
     * @return
     */
    String className();
}
