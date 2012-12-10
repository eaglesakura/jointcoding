package com.eaglesakura.lib.jc.exp.callsupport;

import java.io.StringWriter;

import javassist.CtClass;

import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.SourceCodeWriter;
import com.eaglesakura.lib.jc.codetemp.CodeTemplate;
import com.eaglesakura.lib.jc.imp.ClassConverter;
import com.eaglesakura.lib.jc.imp.FieldConverter;
import com.eaglesakura.lib.jc.imp.MethodConverter;
import com.eaglesakura.lib.jc.imp.MethodConverter.Argment;
import com.eaglesakura.lib.jc.type.JniArgmentType;
import com.eaglesakura.lib.jc.type.JniMethodResultType;

public class CppImplExporter extends CppClassInfomationBase {

    public CppImplExporter(ClassConverter classConverter) {
        super(classConverter);
        listupJCClass();
    }

    public StringBuffer export() {
        SourceCodeWriter writer = new SourceCodeWriter();
        JCUtil.writeHeaderComment(writer);
        // include
        {
            writer.add("#include \"").add(getHeaderName()).add("\"").newline();
            writer.newline();
        }

        // ネームスペース出力
        if (converter.hasCppNamespace()) {
            writer.add("namespace ").add(converter.getCppNamespace()).add(" {").newline();
            writer.newline();
        }

        switch (converter.getMode()) {
            case Protocol:
                exportProtocol(writer);
                break;
            case CallSupport:
                exportCallSupport(writer);
                break;
        }

        // ネームスペース出力
        if (converter.hasCppNamespace()) {
            writer.newline();
            writer.add("}").newline();
            writer.newline();
        }

        writer.flush();
        return writer.getBuffer();
    }

    private void exportProtocol(SourceCodeWriter writer) {

        // 定数フィールドを出力
        exportStaticField(writer);

    }

    private void exportCallSupport(SourceCodeWriter writer) {
        String className = converter.getClassName();
        // JNI用のシグニチャを取得
        {
            String sign = converter.getJavaClassName();
            sign = sign.replaceAll("\\.", "/");
            writer.add("const ::jc::charactor* ").add(converter.getClassName()).add("::CLASS_SIGNATURE = \"").add(sign)
                    .add("\";");
            writer.newline();
        }

        // 定数フィールドを出力
        exportStaticField(writer);

        // 初期化用メソッドを呼び出す
        {
            CodeTemplate template = new CodeTemplate("j2c_impl_initialize.txt");

            template.replase("JCEXPORTCLASS", converter.getClassName());
            template.replase("JCEXPORTMETHOD_LENGTH", "" + methods.size());

            {
                SourceCodeWriter tempWriter = new SourceCodeWriter();
                int methodId = 0;
                for (MethodConverter method : methods) {
                    tempWriter.indent().indent().add(getInitializeImpl(method, methodId)).newline();
                    ++methodId;
                }
                tempWriter.flush();
                template.replase("JCEXPORTMETHODS", tempWriter.toString());
                try {
                    tempWriter.close();
                } catch (Exception e) {

                }
            }

            writer.append(template.getCode());
        }

        // コンストラクタ部分を記述
        {
            writer.newline();
            writer.add(className).add("::").add(className).add("(jobject obj): ").add(superClass).add("(obj)").add("{")
                    .newline();

            writer.indent().add("initialize_").add(className).add("();").newline();
            writer.add("}").newline();
            writer.newline();
        }

        // 実装部分を記述
        {
            int id = 0;
            for (MethodConverter method : methods) {
                writer.add(getImpl(method, id, false).toString()).newline();
                if (!method.isStaticMethod()) {
                    writer.add(getImpl(method, id, true).toString()).newline();
                }
                ++id;
            }
        }

        // wrap
        {
            writer.add("jc_sp<").add(className).add("> ").add(className).add("::wrap(jobject obj) {").newline();
            writer.indent().add("return jc_sp<").add(className).add(">( new ").add(className).add("(obj));").newline();
            writer.add("}").newline();
        }
        writer.newline();
        // global
        {
            writer.add("jc_sp<").add(className).add("> ").add(className).add("::global(jobject obj) {").newline();
            {
                writer.indent().add("return jc_sp<").add(className).add(">( (").add(className).add("*)(new ")
                        .add(className).add("(obj))->addGlobalRef());").newline();
            }
            writer.add("}").newline();
        }

        writer.newline();
        // getClass
        {
            writer.add("jclass ").add(className).add("::getClass() {").newline();
            {
                writer.indent().add("initialize_").add(className).add("();").newline();
                writer.indent().add("return class_").add(className).add(";").newline();
            }
            writer.add("}").newline();
        }

    }

    /**
     * 定数フィールドを出力する
     * @param writer
     */
    private void exportStaticField(SourceCodeWriter writer) {
        for (FieldConverter field : fields) {
            writer.add(getCallImpl(field)).newline();
        }
        writer.newline();
    }

    private String getCallImpl(FieldConverter field) {
        StringWriter writer = new StringWriter();
        writer.append("const ").append(field.getCppTypeName()).append(" ");
        writer.append(converter.getClassName()).append("::").append(field.getName()).append(" = ")
                .append(field.getValue());
        writer.append(";");
        writer.flush();
        return writer.getBuffer().toString();
    }

    /**
     * メソッドID取得部分を追加
     * @param method
     * @param id
     * @return
     */
    private String getInitializeImpl(MethodConverter method, int id) {
        StringWriter writer = new StringWriter();
        String methodArrayName = "methods_" + converter.getClassName();
        {
            writer.append(methodArrayName).append("[").append(Integer.toString(id)).append("] = ");
            writer.append("::ndk::JniWrapper::loadMethod(class_").append(converter.getClassName()).append(", \"")
                    .append(method.getRawName()).append("\"").append(", \"").append(method.getMethodSigneture())
                    .append("\", ").append(Boolean.toString(method.isStaticMethod())).append(");");
        }

        writer.flush();
        return writer.getBuffer().toString();
    }

    /**
     * メソッドの実装部分を出力する
     * @param method
     * @param id
     * @return
     */
    private String getImpl(MethodConverter method, int id, boolean isSupport) {
        SourceCodeWriter writer = new SourceCodeWriter();

        String methodsArrayName = "methods_" + converter.getClassName();
        String className = converter.getClassName();

        writer.append(JCUtil.toCppType(method.getResultType()));
        writer.append(" ").append(className).append("::").append(method.getName());
        if (isSupport) {
            writer.append("_");
        }
        writer.append("(");

        if (isSupport) {
            writer.add("jobject _this");
            if (method.getArgmentNum() > 0) {
                writer.add(", ");
            }
        }

        {

            for (int i = 0; i < method.getArgmentNum(); ++i) {
                Argment arg = method.getArgment(i);
                CtClass paramType = arg.getType();
                String argName = arg.getName();
                writer.append(JCUtil.toCppType(paramType)).append(" ").append(argName);
                if (i < (method.getArgmentNum() - 1)) {
                    writer.append(", ");
                }
            }
        }
        writer.add(") {").newline();
        writer.indent().add("CALL_JNIENV();").newline();
        // staticだったらintializeも読んであげる
        if (method.isStaticMethod() || isSupport) {
            writer.indent().add("initialize_").add(className).add("();").newline();
        }

        writer.indent();

        JniArgmentType resultJniType = JniArgmentType.getType(method.getResultType().getSimpleName());
        JniMethodResultType call = resultJniType.getResultType();
        // Voidメソッド以外ならば、キャストを行って返す
        if (call != JniMethodResultType.Void) {
            writer.append("return (").append(resultJniType.getJniName()).append(") ");
        }
        writer.append("env->").append(call.getCallMethodName(method.isStaticMethod())).append('(');

        if (method.isStaticMethod()) {
            writer.append("class_").append(className).append(", ");
        } else {
            if (isSupport) {
                writer.append("_this, ");
            } else {
                writer.append("this->getObject(), ");
            }
        }
        writer.append(methodsArrayName).append("[").append(Integer.toString(id)).append("]");

        if (method.getArgmentNum() > 0) {
            writer.append(", ");
            // 引数一覧
            for (int i = 0; i < method.getArgmentNum(); ++i) {

                writer.append(method.getArgment(i).getName());
                if (i < (method.getArgmentNum() - 1)) {
                    writer.append(", ");
                }
            }
        }
        writer.add(");").newline();

        writer.add("}").newline();

        if (method.isNativeMethod()) {
            if (method.isFactoryMethod()) {
                writeNativeFactoryMethod(method, writer);
            } else if (method.isAliasMethod()) {
                writeNativeAliasMethod(method, writer);
            } else {
                writeNativeMethodStub(method, writer);
            }
            writer.newline();
        }
        writer.flush();
        return writer.getBuffer().toString();
    }

    /**
     * Nativeメソッド用のスタブを書き出す
     * @param write
     */
    private void writeNativeMethodStub(MethodConverter method, StringWriter writer) {
        CodeTemplate template = new CodeTemplate("j2c_impl_jnicall.txt");

        // メソッド名
        String functionName = converter.getJavaClassName() + "_" + method.getName();
        functionName = functionName.replaceAll("\\.", "_");

        // 引数
        String argments = "";

        // 戻り値の型
        String resultType = JCUtil.toCppType(method.getResultType());

        // headername
        String headerName = getHeaderName();

        if (method.getArgmentNum() > 0) {
            argments += ", ";
            for (int i = 0; i < method.getArgmentNum(); ++i) {
                Argment arg = method.getArgment(i);
                String type = JCUtil.toCppType(arg.getType());
                argments += type;
                argments += " ";
                argments += arg.getName();

                if (i < (method.getArgmentNum() - 1)) {
                    argments += ", ";
                }
            }
        }

        template.replase("JCEXPORTCLASSNATIVEFUNCTIONNAME", functionName);
        template.replase("JCEXPORTCLASS_JNIARGMETNS", argments);
        template.replase("JCRESULTTYPE", resultType);

        if (resultType.equals("void")) {
            template.replase("JCEXPORTCLASSRETURNCODE", "return;");
        } else {
            template.replase("JCEXPORTCLASSRETURNCODE", "return (" + resultType + ") 0;");
        }
        template.replase("JCEXPORTCLASSHEADERNAME", headerName);
        writer.append(template.getCode());
    }

    /**
     * 別名呼び出しのスタブを作成する
     * @param write
     */
    private void writeNativeAliasMethod(MethodConverter method, StringWriter writer) {
        CodeTemplate template = new CodeTemplate("j2c_impl_aliascall.txt");

        // メソッド名
        String functionName = converter.getJavaClassName() + "_" + method.getName();
        functionName = functionName.replaceAll("\\.", "_");

        // 引数
        String argments = "";

        // 戻り値の型
        String resultType = JCUtil.toCppType(method.getResultType());

        // newの引数に与える引数リスト
        String newArgs = "";

        if (method.getArgmentNum() > 1) {
            argments += ", ";
            for (int i = 1; i < method.getArgmentNum(); ++i) {
                Argment arg = method.getArgment(i);
                String type = JCUtil.toCppType(arg.getType());
                argments += type;
                argments += " ";
                argments += arg.getName();

                newArgs += arg.getName();

                if (i < (method.getArgmentNum() - 1)) {
                    argments += ", ";
                    newArgs += ", ";

                }
            }
        }

        template.replase("JCEXPORTCLASSNATIVEFUNCTIONNAME", functionName);
        template.replase("JCEXPORTCLASS_JNIARGMETNS", argments);
        template.replase("JCRESULTTYPE", resultType);
        template.replase("ALIAS_CLASSNAME", method.getAliasMethodClassName());

        if (resultType.equals("void")) {
            template.replase("JCEXPORTCLASSRETURNCODE", "pointer->" + method.getName() + "(" + newArgs + ");");
        } else {
            template.replase("JCEXPORTCLASSRETURNCODE", "return (" + resultType + ") pointer->" + method.getName()
                    + "(" + newArgs + ");");
        }
        writer.append(template.getCode());
    }

    /**
     * ファクトリメソッドを作成する
     * @param write
     */
    private void writeNativeFactoryMethod(MethodConverter method, StringWriter writer) {
        CodeTemplate template = new CodeTemplate("j2c_impl_factory.txt");

        // メソッド名
        String functionName = converter.getJavaClassName() + "_" + method.getName();
        functionName = functionName.replaceAll("\\.", "_");

        // 引数
        String argments = "";

        // 戻り値の型
        String resultType = JCUtil.toCppType(method.getResultType());

        // newの引数に与える引数リスト
        String newArgs = "";

        if (method.getArgmentNum() > 0) {
            argments += ", ";
            for (int i = 0; i < method.getArgmentNum(); ++i) {
                Argment arg = method.getArgment(i);
                String type = JCUtil.toCppType(arg.getType());
                argments += type;
                argments += " ";
                argments += arg.getName();

                newArgs += arg.getName();

                if (i < (method.getArgmentNum() - 1)) {
                    argments += ", ";
                    newArgs += ", ";
                }
            }
        }

        template.replase("JCEXPORTCLASSNATIVEFUNCTIONNAME", functionName);
        template.replase("JCEXPORTCLASS_JNIARGMETNS", argments);
        template.replase("JCRESULTTYPE", resultType);

        template.replase("JCEXPORTCLASS_CALL", method.getFactoryClassName() + "(" + newArgs + ")");
        writer.append(template.getCode());
    }

}
