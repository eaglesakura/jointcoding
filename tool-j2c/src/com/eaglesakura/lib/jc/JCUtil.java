package com.eaglesakura.lib.jc;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.StringWriter;
import java.util.HashMap;
import java.util.Map;

import javassist.CtClass;
import javassist.CtField;
import javassist.CtMethod;

import com.eaglesakura.lib.jc.type.JniArgmentType;

public class JCUtil {

    /**
     * コード出力時のインデント
     */
    public static final String INDENT_SPACE = "    ";

    /**
     * 改行コード
     */
    public static final String NEWLINE = "\n";

    public static Object getAnnotation(CtClass clazz, Class<?> clz) {
        try {
            final String HEADER = "@" + clz.getName();
            Object[] annotations = clazz.getAnnotations();
            for (Object anno : annotations) {
                String str = anno.toString();
                if (str.startsWith(HEADER)) {
                    return anno;
                }
            }
        } catch (Exception e) {
            //            e.printStackTrace();
        }
        return null;
    }

    public static Object getAnnotation(CtField clazz, Class<?> clz) {
        try {
            final String HEADER = "@" + clz.getName();
            Object[] annotations = clazz.getAnnotations();
            for (Object anno : annotations) {
                String str = anno.toString();
                if (str.startsWith(HEADER)) {
                    return anno;
                }
            }
        } catch (Exception e) {
            //            e.printStackTrace();
        }
        return null;
    }

    public static Object getAnnotation(CtMethod clazz, Class<?> clz) {
        try {
            final String HEADER = "@" + clz.getName();
            Object[] annotations = clazz.getAnnotations();
            for (Object anno : annotations) {
                String str = anno.toString();
                if (str.startsWith(HEADER)) {
                    return anno;
                }
            }
        } catch (Exception e) {
            //            e.printStackTrace();
        }
        return null;
    }

    /**
     * アノテーションのパラメータ一覧を返す
     * @param annotation
     * @return
     */
    public static Map<String, String> getAnnotationArgs(Object annotation) {
        Map<String, String> result = new HashMap<String, String>();
        return result;
    }

    public static String toCppType(CtClass clazz) {
        String simpleName = clazz.getSimpleName();

        return JniArgmentType.getType(simpleName).getJniName();
    }

    public static StringWriter writeHeaderComment(StringWriter writer) {
        final String newline = "\n";

        writer.append("/**").append(newline);
        writer.append(" * export from \"Joint Coding Project\"").append(newline);
        writer.append(" */").append(newline);

        writer.flush();
        return writer;
    }

    public static void write(String str, File out) throws IOException {
        FileOutputStream os = new FileOutputStream(out);
        {
            os.write(str.getBytes());
        }
        os.close();
    }
}
