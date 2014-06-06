package com.eaglesakura.lib.jc.imp;

import java.io.ByteArrayInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import javassist.ClassPool;
import javassist.CtClass;

import com.eaglesakura.lib.android.game.util.GameUtil;
import com.eaglesakura.lib.android.game.util.LogUtil;

/**
 * "*.class"ファイルローディングを行う。
 * 
 *
 */
public class ClassesImporter {

    /**
     * ディレクトリから読み込みを行う。
     * 配下全てをクローリングする
     * @param directory
     */
    public static void fromDirectory(File directory) {
        if (directory.isFile()) {
            fromFile(directory.getAbsoluteFile());
            return;
        } else {
            // 下の階層を読み込む
            File[] files = directory.listFiles();
            if (files == null) {
                return;
            }
            for (File file : files) {
                fromDirectory(file);
            }
        }
    }

    /**
     * ファイルから読み込みを行う。
     * @param file
     */
    public static boolean fromFile(File file) {
        {
            String name = file.getName();
            if (name.endsWith(".jar") || name.endsWith(".zip")) {
                return fromJar(file);
            }

            if (!name.endsWith(".class")) {
                return false;
            }
        }
        LogUtil.log("    from file :: " + file.getAbsolutePath());
        InputStream is = null;
        try {
            is = new FileInputStream(file);

            return fromStream(is);
        } catch (Exception e) {
            LogUtil.log(e);
        } finally {
            try {
                is.close();
            } catch (Exception e) {

            }
        }
        return false;
    }

    public static boolean fromStream(InputStream is) throws IOException {
        ClassPool pool = ClassPool.getDefault();
        CtClass cc = pool.makeClass(is);
        return cc != null;
    }

    public static CtClass get(Class<?> clazz) {
        try {
            return ClassPool.getDefault().get(clazz.getName());
        } catch (Exception e) {

        }
        return null;
    }

    /**
     * JARから一括で読み込む
     * @param file
     * @return
     */
    public static boolean fromJar(File file) {
        LogUtil.log("    from jar :: " + file.getAbsolutePath());
        InputStream is = null;
        try {
            int clzCount = 0;
            is = new FileInputStream(file);
            ZipInputStream zis = new ZipInputStream(is);

            ZipEntry entry = null;

            while ((entry = zis.getNextEntry()) != null) {
                String name = entry.getName();
                if (name.endsWith(".class")) {
                    //                    LogUtil.log("    unzip :: " + name);
                    try {
                        byte[] clazzBuffer = GameUtil.toByteArray(zis, false);
                        fromStream(new ByteArrayInputStream(clazzBuffer));
                        ++clzCount;
                    } catch (Exception e) {

                    }
                }
                zis.closeEntry();
            }

            LogUtil.log("  import :: " + clzCount + " classes");
        } catch (Exception e) {
            LogUtil.log(e);
        } finally {
            try {
                is.close();
            } catch (Exception e) {

            }
        }
        return false;
    }
}
