package com.eaglesakura.lib.jc.exp.callsupport;

import java.util.LinkedList;
import java.util.List;

import com.eaglesakura.lib.jc.annotation.jnimake.JCAliasMethod;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;
import com.eaglesakura.lib.jc.imp.ClassConverter;
import com.eaglesakura.lib.jc.imp.FieldConverter;
import com.eaglesakura.lib.jc.imp.MethodConverter;

/**
 * C++用のクラス情報
 * 
 *
 */
public abstract class CppClassInfomationBase {
    /**
     * スーパークラス
     */
    protected String superClass = "::ndk::JniWrapper";

    /**
     * 出力するJavaクラスのシグニチャ
     */
    protected String classSignature = null;

    /**
     * メソッド一覧
     */
    protected List<MethodConverter> methods = new LinkedList<MethodConverter>();

    /**
     * フィールド一覧
     */
    protected List<FieldConverter> fields = new LinkedList<FieldConverter>();

    /**
     * クラス変換
     */
    ClassConverter converter = null;

    protected CppClassInfomationBase(ClassConverter classConverter) {
        converter = classConverter;

        {
            classSignature = converter.getJavaClassName();
            classSignature = classSignature.replaceAll("\\.", "/");
        }

    }

    /**
     * 必要な要素をリストアップする
     */
    protected void listupJCClass() {
        methods.addAll(converter.getMethods(JCMethod.class));
        methods.addAll(converter.getMethods(JCAliasMethod.class));
        fields = converter.getFields(JCField.class);
    }

    public String getHeaderName() {
        return converter.getCppNamespace() + converter.getClassName() + ".h";
    }

    public String getCppName() {
        return converter.getCppNamespace() + converter.getClassName() + ".cpp";
    }
}
