package com.eaglesakura.lib.jc;

import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.util.List;

import javassist.ClassPool;
import javassist.CtClass;

import com.eaglesakura.lib.android.game.util.FileUtil;
import com.eaglesakura.lib.android.game.util.LogUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.exp.callsupport.CppHeaderExporter;
import com.eaglesakura.lib.jc.exp.callsupport.CppImplExporter;
import com.eaglesakura.lib.jc.imp.ClassConverter;
import com.eaglesakura.lib.jc.imp.ClassesImporter;
import com.eaglesakura.lib.jc.util.CmdArgment;
import com.eaglesakura.lib.jc.util.CmdArgmentList;

public class Main {
    /**
     * 出力先ディレクトリ
     */
    static File EXPORT_DIRECTORY = null;

    /**
     * @param args
     */
    public static void main(String[] args) {

        CmdArgmentList argments = new CmdArgmentList(args);

        // ライブラリを読み込む
        {
            CmdArgment params = argments.getArgmentParams("library");
            List<String> list = params.getArgments();
            for (String path : list) {
                LogUtil.log("load class path :: " + path);
                ClassesImporter.fromDirectory(new File(path));
            }
        }

        // 出力先ディレクトリ
        {
            String oututPath = argments.getArgmentParam("output");
            EXPORT_DIRECTORY = new File(oututPath);
            FileUtil.cleanDirectory(EXPORT_DIRECTORY);
            FileUtil.mkdir(EXPORT_DIRECTORY);
        }

        // 入力ディレクトリ
        {
            String inputPath = argments.getArgmentParam("source");
            File IMPORT_DIRECTORY = new File(inputPath);

            ClassesImporter.fromDirectory(IMPORT_DIRECTORY);
            loadDirectory(IMPORT_DIRECTORY);
        }
    }

    static void loadDirectory(File dir) {
        if (dir.isFile()) {
            loadFile(dir);
        } else {
            File[] files = dir.listFiles();
            if (files != null) {
                for (File file : files) {
                    if (file.isDirectory()) {
                        loadDirectory(file);
                    } else {
                        loadFile(file);
                    }
                }
            }
        }
    }

    static void loadFile(File file) {
        try {

            InputStream is = new FileInputStream(file);
            ClassPool pool = ClassPool.getDefault();
            CtClass cc = pool.makeClass(is);

            is.close();

            // クラス自体に付与されたアノテーションを取得
            if (JCUtil.getAnnotation(cc, JCClass.class) != null) {
                // 読み込めたらクラス名を取得
                System.out.println("export = " + cc.getName());

                ClassConverter converter = new ClassConverter(cc);
                {
                    CppHeaderExporter exporter = new CppHeaderExporter(converter);
                    File headerFile = new File(EXPORT_DIRECTORY, exporter.getHeaderName());

                    JCUtil.write(exporter.export().toString(), headerFile);
                }
                {
                    CppImplExporter exporter = new CppImplExporter(converter);
                    File cppFile = new File(EXPORT_DIRECTORY, exporter.getCppName());
                    JCUtil.write(exporter.export().toString(), cppFile);
                }
            }
        } catch (Exception e) {

        }
    }
}
