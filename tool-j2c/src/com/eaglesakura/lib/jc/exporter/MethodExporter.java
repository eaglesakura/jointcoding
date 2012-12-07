package com.eaglesakura.lib.jc.exporter;

import java.io.StringWriter;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.Modifier;
import javassist.bytecode.CodeAttribute;
import javassist.bytecode.LocalVariableAttribute;

import com.eaglesakura.lib.jc.AnnotationArgment;
import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;
import com.eaglesakura.lib.jc.codetemp.CodeTemplate;
import com.eaglesakura.lib.jc.type.JniArgmentType;
import com.eaglesakura.lib.jc.type.JniMethodResultType;

/**
 * メソッドを出力する
 * 
 *
 */
public class MethodExporter {
    ClassExporter parent;
    CtMethod method;

    /**
     * メソッド番号
     */
    int methodId = 0;

    /**
     * メソッド名
     */
    String methodName = null;

    /**
     * メソッドを呼び出すときに必要なシグニチャ
     */
    String callSign = null;

    /**
     * 戻り値のシグニチャ
     */
    CtClass resultType = null;

    /**
     * 引数一覧
     */
    CtClass[] paramTypes = null;

    /**
     * 引数名を保持しておく
     */
    String[] argNames = null;

    /**
     * static属性が付いているならtrue
     */
    boolean isStatic = false;

    AnnotationArgment argments = null;

    public MethodExporter(ClassExporter exporeter, CtMethod method) {
        this.parent = exporeter;
        this.method = method;
        this.argments = new AnnotationArgment(JCUtil.getAnnotation(method, JCMethod.class));
    }

    /**
     * メソッド管理番号を設定する
     * @param methodId
     */
    public void setMethodId(int methodId) {
        this.methodId = methodId;
    }

    /**
     * native呼び出しもエクスポートするならtrue
     * @return
     */
    public boolean isNativeMethod() {
        return argments.getArgment("nativeMethod", false);
    }

    /**
     * 実際の構築を行う
     */
    private void _build() throws Exception {
        methodName = method.getName();
        callSign = method.getSignature();
        resultType = method.getReturnType();
        paramTypes = method.getParameterTypes();
        isStatic = Modifier.isStatic(method.getModifiers());

        // 引数名を保持する
        {
            if (paramTypes != null && paramTypes.length > 0) {
                argNames = new String[paramTypes.length];
                CodeAttribute code = (CodeAttribute) method.getMethodInfo().getAttribute("Code");
                LocalVariableAttribute lval = null;
                if (code != null) {
                    lval = (LocalVariableAttribute) code.getAttribute("LocalVariableTable");
                }

                for (int i = 0; i < paramTypes.length; ++i) {
                    int index = isStatic ? i : (i + 1);
                    if (lval != null) {
                        argNames[i] = lval.getConstPool().getUtf8Info(lval.nameIndex(index));
                    } else {
                        argNames[i] = (paramTypes[i].getSimpleName().toLowerCase() + "_" + i);
                    }
                }
            } else {
                paramTypes = new CtClass[0];
                argNames = new String[0];
            }
        }
        //        System.out.println("sign :: " + callSign);
        System.out.println("    header :: " + getCppHeaderPrototype(false));
    }

    private final String newline = "\n";
    private final String indent = "    ";

    /**
     * メソッドのシグニチャを取得する
     * @return
     */
    public String getMethodSigneture() {
        //        return callSign.replaceAll("/", ".");
        return callSign;
    }

    /**
     * 初期化部分で記述すべき実装部分文字列を取得する
     * @return
     */
    public StringBuffer getInitializeImpl(String methodsArrayName, String className) {
        StringWriter writer = new StringWriter();

        {
            writer.append(methodsArrayName).append("[").append(Integer.toString(methodId)).append("] = ");
            writer.append("JCJniWrapper::loadMethod(").append(className).append(", \"").append(methodName).append("\"")
                    .append(", \"").append(getMethodSigneture()).append("\", ").append(Boolean.toString(isStatic))
                    .append(");");
        }

        writer.flush();
        return writer.getBuffer();
    }

    /**
     * Nativeメソッド用のスタブを書き出す
     * @param write
     */
    private void writeNativeMethodStub(StringWriter writer) {
        CodeTemplate template = new CodeTemplate("j2c_impl_jnicall.txt");

        // メソッド名
        String functionName = parent.javaClassName + "_" + methodName;
        functionName = functionName.replaceAll("\\.", "_");

        // 引数
        String argments = "";

        // 戻り値の型
        String resultType = JCUtil.toCppType(this.resultType);

        // headername
        String headerName = parent.getHeaderName();

        if (paramTypes.length > 0) {
            argments += ", ";
            for (int i = 0; i < paramTypes.length; ++i) {
                String type = JCUtil.toCppType(paramTypes[i]);
                argments += type;
                argments += " ";
                argments += argNames[i];

                if (i < (paramTypes.length - 1)) {
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
        writer.append(template.getCode()).append(newline);
    }

    /**
     * メソッドの呼び出し部分を取得する
     * @return
     */
    public StringBuffer getCallImpl(String methodsArrayName, String className, boolean isSupport) {
        if (isSupport && isStatic) {
            throw new RuntimeException("isSupport && isStatic");
        }

        StringWriter writer = new StringWriter();
        writer.append(JCUtil.toCppType(resultType));
        writer.append(" ").append(parent.className).append("::");
        // サポートメソッドなら頭をつける
        if (isSupport) {
            writer.append("_");
        }
        writer.append(methodName).append("(");
        {
            // サポートメソッドなら、自身をつける
            if (isSupport) {
                writer.write("jobject _this");
                if (paramTypes.length > 0) {
                    writer.write(", ");
                }
            }

            for (int i = 0; i < paramTypes.length; ++i) {
                CtClass paramType = paramTypes[i];
                String argName = argNames[i];

                writer.append(JCUtil.toCppType(paramType)).append(" ").append(argName);
                if (i < (paramTypes.length - 1)) {
                    writer.append(", ");
                }
            }
        }
        writer.append(") {").append(newline);
        writer.append(indent).append("CALL_JNIENV();").append(newline);
        // staticだったらintializeも呼んであげる
        if (isStatic) {
            writer.append(indent).append("initialize_").append(parent.className).append("();").append(newline);
        }

        writer.append(indent);

        JniArgmentType resultJniType = JniArgmentType.getType(resultType.getSimpleName());
        JniMethodResultType call = resultJniType.getResultType();

        // Voidメソッド以外ならば、キャストを行って返す
        if (call != JniMethodResultType.Void) {
            writer.append("return (").append(resultJniType.getJniName()).append(") ");
        }
        writer.append("env->").append(call.getCallMethodName(isStatic)).append('(');

        if (isStatic) {
            writer.append(className).append(", ");
        } else {
            if (isSupport) {
                writer.append("_this, ");
            } else {
                writer.append("this->getObject(), ");
            }
        }
        writer.append(methodsArrayName).append("[").append(Integer.toString(methodId)).append("]");

        if (argNames.length > 0) {
            writer.append(", ");
            // 引数一覧
            for (int i = 0; i < argNames.length; ++i) {
                writer.append(argNames[i]);
                if (i < (argNames.length - 1)) {
                    writer.append(", ");
                }
            }
        }
        writer.append(");").append(newline);

        writer.append("}").append(newline);

        if (isNativeMethod()) {
            writeNativeMethodStub(writer);
        }

        writer.flush();
        return writer.getBuffer();
    }

    /**
     * Static属性ならtrue
     * @return
     */
    public boolean isStaticMethod() {
        return isStatic;
    }

    /**
     * C++のヘッダに書き込むためのプロトタイプ部分を出力する
     * @return
     */
    public String getCppHeaderPrototype(boolean isSupport) {
        String result = "";

        // もともとstaticなら、サポートメソッドは必要ない
        if (isStatic && isSupport) {
            return result;
        }

        if (isStatic || isSupport) {
            result += "static ";
        } else {
            result += "virtual ";
        }
        if (parent.isProtocol) {
            result += JniArgmentType.getType(resultType.getSimpleName()).getFieldType();
        } else {
            result += JCUtil.toCppType(resultType);
        }
        result += " ";
        // サポートメソッドなら、最初に目印を付けておく
        if (isSupport) {
            result += "_";
        }
        result += methodName;
        result += "(";

        // サポートメソッドならjobjectを加える
        if (isSupport) {
            result += "jobject _this";

            // 引数を持っているなら、カンマを付ける
            if (paramTypes.length > 0) {
                result += ", ";
            }
        }

        if (paramTypes.length > 0) {
            for (int i = 0; i < paramTypes.length; ++i) {
                CtClass param = paramTypes[i];

                if (parent.isProtocol) {
                    result += JniArgmentType.getType(param.getSimpleName()).getFieldType();
                } else {
                    result += JCUtil.toCppType(param);
                }
                result += " ";
                result += argNames[i];

                // 最後の1つでなければ、","で引数を繋げる
                if (i != (paramTypes.length - 1)) {
                    result += ", ";
                }

            }
        } else {
            result += "void";
        }
        result += ")";

        return result;
    }

    /**
     * メソッドの基本情報を構築する
     * @return 構築できなかった場合、nullを返す
     */
    public static MethodExporter build(ClassExporter exporter, CtMethod method) {
        try {
            // 親がプロトコルだったら強制出力する
            if (exporter.isProtocol || JCUtil.getAnnotation(method, JCMethod.class) != null) {
                if (exporter.isProtocol) {
                    if (method.getDeclaringClass() != exporter.clazz) {
                        return null;
                    }
                }
                MethodExporter exporeter = new MethodExporter(exporter, method);
                exporeter._build();
                return exporeter;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

    @Override
    public boolean equals(Object obj) {
        String a = method.getSignature();
        String b = ((MethodExporter) obj).getMethodSigneture();
        return a.equals(b);
    }

    @Override
    public int hashCode() {
        return method.getSignature().hashCode();
    }
}
