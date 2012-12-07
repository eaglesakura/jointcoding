package com.eaglesakura.lib.jc.annotation.jnimake;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * nativeコールを行う。
 * 第1引数は必ずポインタ値である必要がある。
 * 
 *
 */
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.CLASS)
public @interface JCAliasMethod {
    /**
     * ポインタの型名を設定する。
     * @return
     */
    String className();
}
