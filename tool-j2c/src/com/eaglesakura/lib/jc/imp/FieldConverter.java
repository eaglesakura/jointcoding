package com.eaglesakura.lib.jc.imp;

import javassist.CtField;

import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.type.JniArgmentType;

public class FieldConverter extends ConverterBase {

    ClassConverter parent;
    CtField field;

    String typeName = "";
    String valueName = "";
    String typeValue = "";

    JniArgmentType jniType;

    protected FieldConverter(ClassConverter parent, CtField field) throws Exception {
        this.parent = parent;
        this.field = field;
        _build();
    }

    private void _build() throws Exception {
        jniType = JniArgmentType.getType(field.getType().getSimpleName());
        this.typeName = jniType.getFieldType();
        this.valueName = field.getName();

        try {
            this.typeValue = field.getConstantValue().toString();
            if (jniType == JniArgmentType.String) {
                typeValue = ("\"" + typeValue + "\"");
            } else if (jniType == JniArgmentType.Float) {
                typeValue = (typeValue + "f");
            }
        } catch (Exception e) {

        }

        System.out.println(indent + typeName + " " + valueName + " = " + typeValue);
    }

    public JniArgmentType getArgmentType() {
        return jniType;
    }

    /**
     * CPP出力する際の型名
     * @return
     */
    public String getCppTypeName() {
        return typeName;
    }

    /**
     * ヘッダのプロトタイプ用の識別子を取得する
     * @return
     */
    public String getFieldPrototype() {
        if (jniType == JniArgmentType.String) {
            return typeName;
        } else {
            return "const " + typeName;
        }
    }

    /**
     * 変数名を取得する
     * @return
     */
    public String getName() {
        return valueName;
    }

    /**
     * 初期値を取得する
     * @return
     */
    public String getValue() {
        return typeValue;
    }

    @Override
    public boolean hasAnnotation(Class<?> anno) {
        return JCUtil.getAnnotation(field, anno) != null;
    }
}
