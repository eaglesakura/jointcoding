package com.eaglesakura.lib.jc.annotation;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

import javassist.ClassPool;
import javassist.CtClass;

import org.junit.Test;

import com.eaglesakura.lib.jc.JCUtil;
import com.eaglesakura.lib.jc.Main;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.imp.ClassConverter;

public class JCClassNameTest {

    @Test
    public void helloTest() {
        System.out.println("hello!");
    }

    @Test
    public void mainTest0() {
        String inputPath = "./bin";
        String outputPath = "./jni";

        Main.main(new String[] {
                inputPath, outputPath
        });
    }

    @Test
    public void exportClassTest() throws Exception {
        InputStream is = new FileInputStream(new File(
                "./bin/com/eaglesakura/lib/jc/sample/jnimake/SampleExportClass.class"));

        ClassPool pool = ClassPool.getDefault();
        CtClass cc = pool.makeClass(is);

        is.close();

        // 読み込めたらクラス名を取得
        System.out.println("ClassName :: " + cc.getName());

        // クラス自体に付与されたアノテーションを取得
        if (JCUtil.getAnnotation(cc, JCClass.class) != null) {
            @SuppressWarnings("unused")
            ClassConverter converter = new ClassConverter(cc);
        }
    }
}
