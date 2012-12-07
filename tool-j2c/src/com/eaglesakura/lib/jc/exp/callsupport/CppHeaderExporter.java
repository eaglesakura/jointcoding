package com.eaglesakura.lib.jc.exp.callsupport;

import java.io.StringWriter;
import java.util.List;

import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.SourceCodeWriter;
import com.eaglesakura.lib.jc.imp.ClassConverter;
import com.eaglesakura.lib.jc.imp.FieldConverter;
import com.eaglesakura.lib.jc.imp.MethodConverter;
import com.eaglesakura.lib.jc.imp.MethodConverter.Argment;
import com.eaglesakura.lib.jc.type.JniArgmentType;

/**
 * C++用のヘッダを書き込むエクスポーター
 * 
 *
 */
public class CppHeaderExporter extends CppClassInfomationBase {
    public CppHeaderExporter(ClassConverter classConverter) {
        super(classConverter);
        listupJCClass();
    }

    public StringBuffer export() {
        SourceCodeWriter writer = new SourceCodeWriter();
        JCUtil.writeHeaderComment(writer);
        final String define = "__" + converter.getClassName().toUpperCase() + "_H__";

        // 多重インクルード防止
        {
            writer.add("#ifndef ").add(define).newline();
            writer.add("#define ").add(define).newline();
        }

        switch (converter.getMode()) {
            case Protocol:
                exportProtocol(writer);
                break;
            default:
                exportCallSupport(writer);
                break;
        }
        // ネームスペース出力
        if (converter.hasCppNamespace()) {
            writer.newline();
            writer.add("}").newline();
            writer.newline();
        }

        // 多重インクルード防止終了
        {
            // define
            writer.add("#endif // ").add(define).newline();
        }

        writer.flush();
        return writer.getBuffer();
    }

    private void exportProtocol(SourceCodeWriter writer) {
        // include部分
        writer.add("#include \"jointcoding.h\"").newline();

        // ネームスペース出力
        if (converter.hasCppNamespace()) {
            writer.add("namespace ").add(converter.getCppNamespace()).add(" {").newline();
            writer.newline();
        }

        String className = converter.getClassName();
        // クラス開始
        {
            writer.add("class ").add(className).add(" {").newline();
        }

        // コンストラクタ・デストラクタ
        {
            writer.add("public:").newline();
            writer.indent().add("virtual ~").add(className).add("(void){").newline();
            writer.indent().add("}").newline();
            writer.newline();
        }

        // メソッド
        exportMethods(writer);

        // フィールド
        exportField(writer);

        // クラスを閉じる
        writer.add("};").newline();
    }

    private void exportCallSupport(SourceCodeWriter writer) {
        List<ClassConverter> protocols = converter.getProtocols();
        // include 部分
        {
            writer.add("#include \"jointcoding-android.h\"").newline();

            //! プロトコルを追加する
            {
                for (ClassConverter protocol : protocols) {
                    CppHeaderExporter _tempEx = new CppHeaderExporter(protocol);
                    writer.add("#include \"").add(_tempEx.getHeaderName()).add("\"").newline();
                }
            }
            writer.newline();
        }

        // ネームスペース出力
        if (converter.hasCppNamespace()) {
            writer.add("namespace ").add(converter.getCppNamespace()).add(" {").newline();
            writer.newline();
        }

        // クラス開始
        {
            writer.add("class ").add(converter.getClassName()).add(": public ").add(superClass);
            //! add protocols
            {
                for (ClassConverter protocol : protocols) {
                    writer.append(", public ").append(protocol.getClassName());
                }
            }
            writer.add(" {").newline();
        }

        // コンストラクタ・デストラクタ
        writer.add("protected:").newline();
        {
            writer.indent().add(converter.getClassName()).add("(jobject obj);").newline();
            writer.add("public:").newline();
            writer.indent().add("virtual ~").add(converter.getClassName()).add("(void){").newline();
            writer.indent().add("}").newline();
            writer.newline();
        }

        // クラス識別子
        {
            writer.indent().add("static const ::jc::charactor* CLASS_SIGNATURE;").newline(false);
        }

        // メソッド
        exportMethods(writer);

        // フィールド
        exportField(writer);

        // 生成用のメソッド
        {
            writer.newline();
            writer.indent().add("static jc_sp<").add(converter.getClassName()).add("> wrap(jobject obj);").newline();
            writer.indent().add("static jc_sp<").add(converter.getClassName()).add("> global(jobject obj);").newline();
        }

        // クラスへのアクセサ
        {
            writer.newline();
            writer.indent().add("static jclass getClass();").newline();
        }
        writer.add("};").newline();
    }

    private void exportMethods(SourceCodeWriter writer) {
        // add methods
        for (MethodConverter method : methods) {
            if (converter.isProtocolMode()) {
                writer.indent().append(getMethodPrototype(method, false));
                writer.append(" = 0");
                writer.add(";").newline();
            } else {
                if (method.isStaticMethod()) {
                    writer.indent().append(getMethodPrototype(method, false));
                    writer.add(";").newline();
                } else {
                    writer.indent().append(getMethodPrototype(method, false));
                    writer.add(";").newline();
                    writer.indent().append(getMethodPrototype(method, true));
                    writer.add(";").newline();
                }
            }
        }
    }

    /**
     * フィールド部分を出力する
     * @param writer
     */
    private void exportField(SourceCodeWriter writer) {
        writer.newline();
        for (FieldConverter field : fields) {
            writer.indent().add(getFieldPrototype(field)).newline();
        }
    }

    private String getMethodPrototype(MethodConverter method, boolean isSupport) {
        String result = "";

        if (isSupport && method.isStaticMethod()) {
            throw new RuntimeException("isSupport && method.isStaticMethod()");
        }

        if (method.isStaticMethod() || isSupport) {
            result += "static ";
        } else {
            result += "virtual ";
        }
        if (converter.isProtocolMode()) {
            result += JniArgmentType.getType(method.getResultType().getSimpleName()).getFieldType();
        } else {
            result += JCUtil.toCppType(method.getResultType());
        }
        result += " ";
        result += method.getName();
        if (isSupport) {
            result += "_";
        }
        result += "(";

        if (isSupport) {
            result += "jobject _this";
            if (method.getArgmentNum() > 0) {
                result += ", ";
            }
        }
        if (method.getArgmentNum() > 0) {
            for (int i = 0; i < method.getArgmentNum(); ++i) {
                Argment arg = method.getArgment(i);
                if (converter.isProtocolMode()) {
                    result += JniArgmentType.getType(arg.getType().getSimpleName()).getFieldType();
                } else {
                    result += JCUtil.toCppType(arg.getType());
                }
                result += " ";
                result += arg.getName();

                // 最後の1つでなければ、","で引数を繋げる
                if (i != (method.getArgmentNum() - 1)) {
                    result += ", ";
                }
            }
        }
        result += ")";

        return result;
    }

    private String getFieldPrototype(FieldConverter field) {
        StringWriter writer = new StringWriter();
        writer.append("static const ").append(field.getCppTypeName()).append(" ").append(field.getName()).append(";");
        writer.flush();
        return writer.getBuffer().toString();
    }
}
