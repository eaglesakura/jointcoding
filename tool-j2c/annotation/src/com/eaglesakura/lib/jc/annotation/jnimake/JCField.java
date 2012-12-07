package com.eaglesakura.lib.jc.annotation.jnimake;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 固定値を出力するために利用する
 * 
 *
 */
@Target(ElementType.FIELD)
@Retention(RetentionPolicy.CLASS)
public @interface JCField {
}
