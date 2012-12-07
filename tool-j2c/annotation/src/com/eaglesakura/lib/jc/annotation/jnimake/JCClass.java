package com.eaglesakura.lib.jc.annotation.jnimake;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Java <--> C++コンバートを行うためのアノテーションフラグ
 * 
 *
 */
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.CLASS)
public @interface JCClass {
    /**
     * 生成されるクラス名を定義する
     * デフォルトの場合、クラス名がそのまま使われる
     * @return
     */
    String className() default "";

    /**
     * C++出力時に利用されるネームスペース。
     * 現時点では１階層のみで出力可能
     * @return
     */
    String cppNamespace() default "";

    /**
     * 別クラスとして参照させる（スタブとして作成する等）場合は設定を行える。
     * パッケージ名も含めて設定する
     * @return
     */
    String aliasClassName() default "";

    /**
     * 出力モードを設定する
     * @return
     */
    JCClass.Mode mode() default JCClass.Mode.CallSupport;

    enum Mode {
        /**
         * プロトコルモードとして定義されている
         * 実装のないインターフェース（純粋仮想クラス）として出力される。
         */
        Protocol,

        /**
         * Java <--> C++を繋ぐブリッジとして定義されている
         */
        Bridge,

        /**
         * Jniの呼び出しヘルパとして定義されている
         */
        CallSupport,
    }
}
