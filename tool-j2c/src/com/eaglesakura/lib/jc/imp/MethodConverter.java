package com.eaglesakura.lib.jc.imp;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.Modifier;
import javassist.bytecode.CodeAttribute;
import javassist.bytecode.LocalVariableAttribute;

import com.eaglesakura.lib.jc.AnnotationArgment;
import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCAliasMethod;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;
import com.eaglesakura.lib.jc.type.JniArgmentType;

/**
 * メソッドを出力する
 * 
 *
 */
public class MethodConverter extends ConverterBase {
    ClassConverter parent;
    CtMethod method;

    /**
     * メソッド名
     */
    String methodName = null;

    /**
     * メソッドを呼び出すときに必要なシグニチャ
     */
    String callSign = null;

    /**
     * 生成するクラス名
     */
    String factoryClassName = "";

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

    /**
     * エイリアスメソッドのパラメーター
     */
    AnnotationArgment aliasMethodArgs = null;

    public MethodConverter(ClassConverter exporeter, CtMethod method) throws Exception {
        this.parent = exporeter;
        this.method = method;
        this.argments = new AnnotationArgment(JCUtil.getAnnotation(method, JCMethod.class));
        this.aliasMethodArgs = new AnnotationArgment(JCUtil.getAnnotation(method, JCAliasMethod.class));
        _build();
    }

    /**
     * native呼び出しもエクスポートするならtrue
     * @return
     */
    public boolean isNativeMethod() {
        return argments.getArgment("nativeMethod", false) || isFactoryMethod() || isAliasMethod();
    }

    /**
     * 別名呼び出しを利用する場合はtrue
     * @return
     */
    public boolean isAliasMethod() {
        return hasAnnotation(JCAliasMethod.class);
    }

    /**
     * 別名呼び出しのメソッド名を取得する
     * @return
     */
    public String getAliasMethodClassName() {
        return aliasMethodArgs.getArgment("className", "");
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
        factoryClassName = argments.getArgment("factory", "");
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
                        argNames[i] = ("arg" + i);
                        try {
                            argNames[i] = (paramTypes[i].getSimpleName().toLowerCase() + "_" + i);
                        } catch (Exception e) {

                        }
                    }
                }
            } else {
                paramTypes = new CtClass[0];
                argNames = new String[0];
            }
        }

        // jobject関係はunsafeの警告を入れる
        try {
            String typename = JniArgmentType.getType(resultType.getSimpleName()).name().toLowerCase();
            if (typename.contains("object") || typename.contains("array")) {
                methodName = methodName + "_unsafe";
            }
        } catch (Exception e) {

        }

    }

    /**
     * メソッドのシグニチャを取得する
     * @return
     */
    public String getMethodSigneture() {
        //        return callSign.replaceAll("/", ".");
        return callSign;
    }

    public String getFactoryClassName() {
        return factoryClassName;
    }

    public boolean isFactoryMethod() {
        return !factoryClassName.isEmpty();
    }

    /**
     * Static属性ならtrue
     * @return
     */
    public boolean isStaticMethod() {
        return isStatic;
    }

    public CtClass getResultType() {
        return resultType;
    }

    public String getName() {
        return methodName;
    }

    public String getRawName() {
        return method.getName();
    }

    public int getArgmentNum() {
        return paramTypes.length;
    }

    public static class Argment {
        private CtClass type;
        private String name;

        public CtClass getType() {
            return type;
        }

        public String getName() {
            return name;
        }
    }

    public Argment getArgment(int index) {
        Argment result = new Argment();
        result.type = paramTypes[index];
        result.name = argNames[index];

        return result;
    }

    @Override
    public boolean equals(Object obj) {
        String a = method.getSignature();
        String b = ((MethodConverter) obj).getMethodSigneture();
        return a.equals(b);
    }

    @Override
    public int hashCode() {
        return method.getSignature().hashCode();
    }

    @Override
    public boolean hasAnnotation(Class<?> anno) {
        return JCUtil.getAnnotation(method, anno) != null;
    }
}
