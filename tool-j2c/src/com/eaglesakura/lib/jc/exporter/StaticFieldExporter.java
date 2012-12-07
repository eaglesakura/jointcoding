package com.eaglesakura.lib.jc.exporter;

import java.io.StringWriter;

import javassist.CtField;

import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCField;
import com.eaglesakura.lib.jc.type.JniArgmentType;

public class StaticFieldExporter {

    //    private final String newline = "\n";
    private final String indent = "    ";

    ClassExporter parent;
    CtField field;

    String typeName = "";
    String valueName = "";
    String typeValue = "";

    protected StaticFieldExporter(ClassExporter parent, CtField field) {
        this.parent = parent;
        this.field = field;
    }

    private void _build() throws Exception {
        JniArgmentType jniType = JniArgmentType.getType(field.getType().getSimpleName());
        this.typeName = jniType.getFieldType();
        this.valueName = field.getName();
        this.typeValue = field.getConstantValue().toString();
        if (jniType == JniArgmentType.String) {
            typeValue = ("\"" + typeValue + "\"");
        } else if (jniType == JniArgmentType.Float) {
            typeValue = (typeValue + "f");
        }

        System.out.println(indent + typeName + " " + valueName + " = " + typeValue);
    }

    /**
     * ヘッダ用のプロトタイプ宣言を取得する
     * @return
     */
    public String getCppHeaderPrototype() {
        StringWriter writer = new StringWriter();
        writer.append("static const ").append(typeName).append(" ").append(valueName).append(";");
        writer.flush();
        return writer.getBuffer().toString();
    }

    public String getCallImpl() {
        StringWriter writer = new StringWriter();
        writer.append("const ").append(typeName).append(" ");
        writer.append(parent.className).append("::").append(valueName).append(" = ").append(typeValue);
        writer.append(";");
        writer.flush();
        return writer.getBuffer().toString();
    }

    /**
     * メソッドの基本情報を構築する
     * @return 構築できなかった場合、nullを返す
     */
    public static StaticFieldExporter build(ClassExporter exporter, CtField field) {
        try {
            // 親がプロトコルだったら絶対に出力する
            if (exporter.isProtocol || JCUtil.getAnnotation(field, JCField.class) != null) {
                if (!JniArgmentType.getType(field.getType().getSimpleName()).isFieldSupport()) {
                    System.out.println("this type :: notsupport!! = " + field.getName());
                    return null;
                }
                StaticFieldExporter exporeter = new StaticFieldExporter(exporter, field);
                exporeter._build();
                return exporeter;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
}
