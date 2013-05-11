package com.eaglesakura.jc.jni.io;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.os.Environment;

import com.eaglesakura.jc.jni.context.NativeContext;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "ndk")
public class NativeIOUtil {

    /**
     * アセットからのファイルを開く
     * @param fileName
     * @param appContext
     * @return
     * @throws IOException
     */
    @JCMethod
    public static InputStream openFromAssets(String fileName, Context appContext) throws IOException {
        try {
            InputStream is = appContext.getResources().getAssets().open(fileName);
            return is;
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * SDカードからファイルを開く
     * @param path
     * @param appContext
     * @return
     * @throws IOException
     */
    @JCMethod
    public static InputStream openFromExternalStrage(String path, Context appContext) throws IOException {
        try {
            File file = new File(Environment.getExternalStorageDirectory(), path);
            if (!file.isFile()) {
                return null;
            }
            return new FileInputStream(file);
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * ローカルパスを実際のパスに変換する
     * @param path
     * @return
     */
    @JCMethod
    public static String localStoragePath2NativePath(String path) {
        File directory = NativeContext.getApplicationContext().getFilesDir().getParentFile();
        File file = new File(directory, path);
        return file.getAbsolutePath();
    }

    /**
     * SDカードからファイルを開く
     * @param path
     * @param appContext
     * @return
     * @throws IOException
     */
    @JCMethod
    public static InputStream openFromLocalStrage(String path, Context appContext) throws IOException {
        File directory = appContext.getFilesDir().getParentFile();
        try {
            File file = new File(directory, path);
            if (!file.isFile()) {
                return null;
            }
            return new FileInputStream(file);
        } catch (Exception e) {
            return null;
        }
    }
}
