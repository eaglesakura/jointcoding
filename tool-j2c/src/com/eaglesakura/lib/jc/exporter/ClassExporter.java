package com.eaglesakura.lib.jc.exporter;

import java.io.StringWriter;
import java.util.LinkedList;
import java.util.List;

import javassist.CtClass;
import javassist.CtField;
import javassist.CtMethod;

import com.eaglesakura.lib.jc.AnnotationArgment;
import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.codetemp.CodeTemplate;

/**
 * クラスのエクスポートを行う。
 * 
 *
 */
public class ClassExporter {
    /**
     * 出力するクラス名
     */
    String className = "";

    /**
     * Java側のクラスフルネーム
     */
    String javaClassName = "";

    /**
     * 出力するクラス
     */
    CtClass clazz = null;

    /**
     * メソッド一覧
     */
    List<MethodExporter> methods = new LinkedList<MethodExporter>();

    /**
     * フィールド値一覧
     */
    List<StaticFieldExporter> fields = new LinkedList<StaticFieldExporter>();

    /**
     * 実装されているプロトコル一覧
     */
    List<CtClass> protocols = new LinkedList<CtClass>();

    boolean isProtocol = false;
    AnnotationArgment argments = null;

    public ClassExporter(CtClass clazz) {
        this.clazz = clazz;
        argments = new AnnotationArgment(JCUtil.getAnnotation(clazz, JCClass.class));
    }

    public String getHeaderName() {
        return className + ".h";
    }

    public String getCppName() {
        return className + ".cpp";
    }

    /**
     * クラス情報をまとめる。
     */
    public void build() {
        className = clazz.getSimpleName();
        javaClassName = clazz.getName();

        className = argments.getArgment("className", className);
        javaClassName = argments.getArgment("aliasClassName", javaClassName);

        isProtocol = argments.getArgment("protocol", false);

        try {
            CtClass[] ctClasses = clazz.getInterfaces();
            for (CtClass clz : ctClasses) {
                AnnotationArgment argment = new AnnotationArgment(JCUtil.getAnnotation(clz, JCClass.class));
                if (argment.getArgment("protocol", false)) {
                    System.out.println("find protocol :: " + clz.getSimpleName());
                    protocols.add(clz);
                }
            }
        } catch (Exception e) {

        }

        // メソッド一覧をチェックする
        {
            CtMethod[] ctMethods = clazz.getMethods();

            for (CtMethod method : ctMethods) {
                MethodExporter exporter = MethodExporter.build(this, method);
                if (exporter != null) {
                    exporter.setMethodId(methods.size());
                    methods.add(exporter);
                }
            }
        }
        // フィールド一覧をチェックする
        {
            CtField[] ctFields = clazz.getFields();
            for (CtField field : ctFields) {
                StaticFieldExporter exporeter = StaticFieldExporter.build(this, field);
                if (exporeter != null) {
                    fields.add(exporeter);
                }
            }
        }
    }

    private final String newline = "\n";
    private final String indent = "    ";

    /**
     * C++用のヘッダを取得する
     * @return
     */
    public StringBuffer exportCppHeader() {
        StringWriter writer = new StringWriter();

        JCUtil.writeHeaderComment(writer);
        final String define = "__" + className.toUpperCase() + "_H__";

        // 多重インクルード防止
        {
            writer.append("#ifndef ").append(define).append(newline);
            writer.append("#define ").append(define).append(newline);
        }

        // include
        {
            if (isProtocol) {
                writer.append("#include \"jointcoding.h\"");
            } else {
                writer.append("#include \"jointcoding-android.h\"");
            }
            writer.append(newline);

            //! add protocols
            {
                for (CtClass protocol : protocols) {
                    ClassExporter ex = new ClassExporter(protocol);
                    ex.build();
                    writer.append("#include \"").append(ex.getHeaderName()).append("\"").append(newline);
                }
            }
            writer.append(newline);
        }

        // class-begin
        if (isProtocol) {
            writer.append("class ").append(className).append(" {").append(newline);
        } else {
            writer.append("class ").append(className).append(": public JCJniWrapper");
            //! add protocols
            {
                for (CtClass protocol : protocols) {
                    ClassExporter ex = new ClassExporter(protocol);
                    ex.build();
                    writer.append(", public ").append(ex.className);
                }
            }
            writer.append(" {").append(newline);
        }
        // コンストラクタ / デストラクタ
        {
            if (!isProtocol) {
                writer.append("protected:").append(newline);
                writer.append(indent).append(className).append("(jobject obj);").append(newline);
            }
            writer.append("public:").append(newline);
            writer.append(indent).append("virtual ~").append(className).append("(void){").append(newline);
            writer.append(indent).append("}").append(newline);
            writer.append(newline);
        }

        // クラス識別子の固定文字列を出力
        if (!isProtocol) {
            writer.append(indent).append("static const charactor* CLASS_SIGNATURE;").append(newline).append(newline);
        }

        // add methods
        for (MethodExporter method : methods) {
            if (isProtocol) {
                writer.append(indent).append(method.getCppHeaderPrototype(false));
                writer.append(" = 0");
            } else {
                writer.append(indent).append(method.getCppHeaderPrototype(false));
                if (!method.isStaticMethod()) {
                    writer.append(";").append(newline);
                    writer.append(indent).append(method.getCppHeaderPrototype(true));
                }
            }
            writer.append(";").append(newline);
        }

        // add field
        {
            writer.append(newline);
            for (StaticFieldExporter field : fields) {
                writer.append(indent).append(field.getCppHeaderPrototype()).append(newline);
            }
        }

        // デフォルトで必要なオーバーライド
        if (!isProtocol) {
            writer.append(newline);
            writer.append(indent).append("JCOBJECT_GETFILENAME()").append(newline);
        }

        // インスタンス作成
        if (!isProtocol) {
            writer.append(newline);
            writer.append(indent).append("static ").append(className).append("* wrap(jobject obj);").append(newline);
            writer.append(indent).append("static ").append(className).append("* global(jobject obj);").append(newline);
        }

        // クラスへのアクセサ
        if (!isProtocol) {
            writer.append(newline);
            writer.append(indent).append("static jclass getClass();").append(newline);
        }
        writer.append("};").append(newline);

        {
            // define
            writer.append("#endif // ").append(define).append(newline);
        }

        writer.flush();
        return writer.getBuffer();
    }

    void exportInitializeMethod(StringWriter writer) {
        CodeTemplate template = new CodeTemplate("j2c_impl_initialize.txt");

        template.replase("JCEXPORTCLASS", className);
        template.replase("JCEXPORTMETHOD_LENGTH", "" + methods.size());

        {
            StringWriter tempWriter = new StringWriter();
            for (MethodExporter method : methods) {
                tempWriter.append(indent).append(indent)
                        .append(method.getInitializeImpl("methods_" + className, "class_" + className)).append(newline);
            }

            template.replase("JCEXPORTMETHODS", tempWriter.toString());
        }

        writer.append(template.getCode());
    }

    void exportCreateMethod(StringWriter writer) {
        // wrap
        {
            writer.append(className).append("* ").append(className).append("::wrap(jobject obj) {").append(newline);
            writer.append(indent).append("return new ").append(className).append("(obj);").append(newline);
            writer.append("}").append(newline);
        }
        writer.append(newline);
        // global
        {
            writer.append(className).append("* ").append(className).append("::global(jobject obj) {").append(newline);
            writer.append(indent).append("return (").append(className).append("*) (new ").append(className)
                    .append("(obj))->addGlobalRef();").append(newline);
            writer.append("}").append(newline);
        }
    }

    /**
     * 実装部分を出力する
     * @return
     */
    public StringBuffer exportCppImpl() {
        StringWriter writer = new StringWriter();

        // 定型句を出力
        {
            JCUtil.writeHeaderComment(writer);
        }

        // include
        {
            writer.append("#include \"").append(getHeaderName()).append("\"").append(newline).append(newline);
        }

        // 定数部分を出力
        if (!isProtocol) {
            String sign = javaClassName;
            sign = sign.replaceAll("\\.", "/");
            writer.append("const charactor* ").append(className).append("::CLASS_SIGNATURE = \"").append(sign)
                    .append("\";");
            writer.append(newline);
        }

        // 定数フィールドを追加
        {
            for (StaticFieldExporter field : fields) {
                writer.append(field.getCallImpl()).append(newline);
            }
            writer.append(newline);
        }

        if (!isProtocol) {
            exportInitializeMethod(writer);

            // コンストラクタ部分を記述
            {
                writer.append(newline);
                writer.append(className).append("::").append(className).append("(jobject obj): JCJniWrapper(obj)")
                        .append("{").append(newline);

                writer.append(indent).append("initialize_").append(className).append("();").append(newline);
                writer.append("}").append(newline).append(newline);
            }

            // 実装部分を記述
            {
                for (MethodExporter method : methods) {
                    writer.append(method.getCallImpl("methods_" + className, "class_" + className, false).toString())
                            .append(newline);
                    if (!method.isStaticMethod()) {
                        writer.append(method.getCallImpl("methods_" + className, "class_" + className, true).toString())
                                .append(newline);
                    }
                }
            }

            // インスタンス作成用のメソッドを出力
            exportCreateMethod(writer);

            // クラス取得部分を追加
            {
                writer.append(newline);
                writer.append("jclass ").append(className).append("::getClass() {").append(newline);
                writer.append(indent).append("initialize_").append(className).append("();").append(newline);
                writer.append(indent).append("return ").append("class_").append(className).append(";").append(newline);
                writer.append("}").append(newline);
            }
        }

        writer.flush();
        return writer.getBuffer();
    }
}
