package com.eaglesakura.jc.android;

/**
 * Proguard対策用のインターフェース.<BR>
 * このインターフェースを実装しているクラスはproguardの適用から外すことを推奨する.<BR>
 * <BR>
 * proguardによってメソッド名等が変わってリフレクションを設定できなくなると、Native側で落ちる危険性が大きい。
 */
public interface IJointCodingClass {

}
