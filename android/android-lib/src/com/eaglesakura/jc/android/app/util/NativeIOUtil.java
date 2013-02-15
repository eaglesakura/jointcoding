package com.eaglesakura.jc.android.app.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import android.content.Context;
import android.os.Environment;

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
        InputStream is = appContext.getResources().getAssets().open(fileName);
        return is;
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
        return new FileInputStream(new File(Environment.getExternalStorageDirectory(), path));
    }
}
