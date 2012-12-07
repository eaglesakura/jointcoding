package com.eaglesakura.lib.jc.annotation.jnimake;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Java -> C++コンバートされるメソッドを定義する
 * 
 *
 */
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.CLASS)
public @interface JCMethod {
    /**
     * nativeメソッドとして認識させるならtrue
     * trueの場合、native用のメソッドも同時に書きだす
     * @return
     */
    boolean nativeMethod() default false;

    /**
     * ファクトリメソッドとして生成する場合は、生成対象のクラス名を設定する
     * @return
     */
    String factory() default "";

    /**
     * nativeの特定メソッドを呼び出す。
     * 第1引数は必ずポインタとして扱う
     * @return
     */
    String aliasCall() default "";
}
