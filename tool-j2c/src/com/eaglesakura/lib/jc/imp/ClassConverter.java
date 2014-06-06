package com.eaglesakura.lib.jc.imp;

import java.util.LinkedList;
import java.util.List;

import javassist.CtClass;
import javassist.CtField;
import javassist.CtMethod;

import com.eaglesakura.lib.android.game.util.GameUtil;
import com.eaglesakura.lib.android.game.util.LogUtil;
import com.eaglesakura.lib.jc.AnnotationArgment;
import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass.Mode;

/**
 * クラスのエクスポートを行う。
 * 
 *
 */
public class ClassConverter extends ConverterBase {
    /**
     * 出力するクラス名
     */
    String className = "";

    /**
     * C++側のネームスペース
     */
    String cppNamespace = "";

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
    List<MethodConverter> methods = new LinkedList<MethodConverter>();

    /**
     * フィールド値一覧
     */
    List<FieldConverter> fields = new LinkedList<FieldConverter>();

    /**
     * 実装されているプロトコル一覧
     */
    List<ClassConverter> protocols = new LinkedList<ClassConverter>();

    /**
     * 変換モード
     */
    JCClass.Mode mode = Mode.CallSupport;

    AnnotationArgment argments = null;

    public ClassConverter(CtClass clazz) {
        this.clazz = clazz;
        argments = new AnnotationArgment(JCUtil.getAnnotation(clazz, JCClass.class));
        _build();
    }

    /**
     * プロトコルモードとして動作する場合はtrue
     * @return
     */
    public boolean isProtocolMode() {
        return mode == Mode.Protocol;
    }

    /**
     * C++ブリッジモードとして動作する場合はtrue
     * @return
     */
    public boolean isBridgeMode() {
        return mode == Mode.Bridge;
    }

    /**
     * JNI呼び出し用クラスをエクスポートする場合はtrue
     * @return
     */
    public boolean isExportMode() {
        return mode == Mode.CallSupport;
    }

    /**
     * 動作モードを取得する
     * @return
     */
    public JCClass.Mode getMode() {
        return mode;
    }

    /**
     * Java上で扱うクラス名を取得する
     * @return
     */
    public String getJavaClassName() {
        return javaClassName;
    }

    public String getClassName() {
        return className;
    }

    /**
     * C++側にネームスペースを持っていたらtrue
     * @return
     */
    public boolean hasCppNamespace() {
        return !GameUtil.isEmpty(cppNamespace);
    }

    public String getCppNamespace() {
        return cppNamespace;
    }

    /**
     * プロトコル一覧を取得する
     * @return
     */
    public List<ClassConverter> getProtocols() {
        return protocols;
    }

    /**
     * 特定アノテーションのついたメソッドを取得する
     * @param annotation
     * @return
     */
    public List<MethodConverter> getMethods(Class<?> annotation) {
        List<MethodConverter> result = new LinkedList<MethodConverter>();
        for (MethodConverter method : methods) {
            if (method.hasAnnotation(annotation)) {
                result.add(method);
            }
        }
        return result;
    }

    /**
     * 特定のアノテーションの付いたフィールドを取得する
     * @param annotation
     * @return
     */
    public List<FieldConverter> getFields(Class<?> annotation) {
        List<FieldConverter> result = new LinkedList<FieldConverter>();
        for (FieldConverter field : fields) {
            if (field.hasAnnotation(annotation)) {
                result.add(field);
            }
        }
        return result;
    }

    /**
     * クラス情報をまとめる。
     */
    private void _build() {
        className = clazz.getSimpleName();
        javaClassName = clazz.getName();

        className = argments.getArgment("className", className);
        javaClassName = argments.getArgment("aliasClassName", javaClassName);

        cppNamespace = argments.getArgment("cppNamespace", "");

        try {
            CtClass[] ctClasses = clazz.getInterfaces();
            for (CtClass clz : ctClasses) {
                AnnotationArgment argment = new AnnotationArgment(JCUtil.getAnnotation(clz, JCClass.class));
                JCClass.Mode mode = Mode.Protocol;
                if (argment.isArgmentEquals("mode", mode.getClass().getName() + "." + mode.name())) {
                    System.out.println("find protocol :: " + clz.getSimpleName());
                    protocols.add(new ClassConverter(clz));
                }
            }
        } catch (Exception e) {

        }

        // コンバートモードを取得する
        {
            AnnotationArgment argment = new AnnotationArgment(JCUtil.getAnnotation(clazz, JCClass.class));
            mode = argment.getEnum("mode", JCClass.Mode.CallSupport);
            LogUtil.log("argment :: " + mode);
        }

        {
            CtClass _current = clazz;
            //            while (_current != null)
            {
                // メソッド一覧をチェックする
                {
                    CtMethod[] ctMethods = _current.getMethods();

                    for (CtMethod method : ctMethods) {
                        try {
                            MethodConverter exporter = new MethodConverter(this, method);
                            methods.add(exporter);
                        } catch (Exception e) {
                            LogUtil.log("fail :: " + method.getName());
                            LogUtil.log(e);
                        }
                    }
                }

                // フィールド一覧をチェックする
                {
                    CtField[] ctFields = _current.getFields();
                    for (CtField field : ctFields) {
                        try {
                            FieldConverter exporeter = new FieldConverter(this, field);
                            fields.add(exporeter);
                        } catch (Exception e) {

                        }
                    }
                }

                try {
                    _current = _current.getSuperclass();
                    LogUtil.log("export = super::" + _current.getName());
                } catch (Exception e) {
                    _current = null;
                }
            }
        }

    }

    @Override
    public boolean hasAnnotation(Class<?> anno) {
        return JCUtil.getAnnotation(clazz, anno) != null;
    }
}
