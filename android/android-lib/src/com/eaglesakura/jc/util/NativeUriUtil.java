package com.eaglesakura.jc.util;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.content.Context;
import android.net.Uri;
import android.os.Environment;

import com.eaglesakura.jc.jni.context.NativeContext;
import com.eaglesakura.jcprotocol.UriProtocol;

public class NativeUriUtil {

    /**
     * NativeUriからSchemeを取得する
     * @param nativeUri
     * @return
     */
    public static String getScheme(String nativeUri) {
        final int index = nativeUri.indexOf("://");
        if (index < 0) {
            return null;
        }

        return nativeUri.substring(0, index + 3);
    }

    /**
     * NativeUriからパスを取得する
     * @param nativeUri
     * @return
     */
    public static String getPath(String nativeUri) {
        final int index = nativeUri.indexOf("://");
        if (index < 0) {
            return null;
        }

        return nativeUri.substring(index + 3);
    }

    private static File buildPath(File root, String path) {
        return new File(root, path);
    }

    /**
     * 入力ストリームを開く
     * @param context
     * @param nativeUri
     * @return
     * @throws IOException
     */
    public static InputStream openInputStream(Context context, String nativeUri) throws IOException {
        final String scheme = getScheme(nativeUri);
        final String path = getPath(nativeUri);

        if (AndroidUtil.isEmpty(scheme) || AndroidUtil.isEmpty(path)) {
            return null;
        }

        if (UriProtocol.SCHEME_APPLI_ASSETS.equals(scheme)) {
            // アセット生成
            return context.getAssets().open(path);
        } else if (UriProtocol.SCHEME_EXTERNALSTRAGE.equals(scheme)) {
            // 外部ストレージ
            final File fullPath = buildPath(Environment.getExternalStorageDirectory(), path);
            return new FileInputStream(fullPath);
        } else if (UriProtocol.SCHEME_LOCALSTRAGE.equals(scheme)) {
            // ローカルストレージ
            final File fullPath = buildPath(context.getFilesDir(), path);
            return new FileInputStream(fullPath);
        } else {
            // その他のパース
            return context.getContentResolver().openInputStream(Uri.parse(nativeUri));
        }
    }

    /**
     * 出力ストリームを開く
     * @param context
     * @param nativeUri
     * @return
     * @throws IOException
     */
    public static OutputStream openOutputStream(Context context, String nativeUri) throws IOException {
        final String scheme = getScheme(nativeUri);
        final String path = getPath(nativeUri);

        if (AndroidUtil.isEmpty(scheme) || AndroidUtil.isEmpty(path)) {
            return null;
        }

        if (UriProtocol.SCHEME_APPLI_ASSETS.equals(scheme)) {
            // アセット生成
            throw new IllegalArgumentException("Uri read only :: " + nativeUri);
        } else if (UriProtocol.SCHEME_EXTERNALSTRAGE.equals(scheme)) {
            // 外部ストレージ
            final File fullPath = buildPath(Environment.getExternalStorageDirectory(), path);
            return new FileOutputStream(fullPath);
        } else if (UriProtocol.SCHEME_LOCALSTRAGE.equals(scheme)) {
            // ローカルストレージ
            final File fullPath = buildPath(context.getFilesDir(), path);
            return new FileOutputStream(fullPath);
        } else {
            // その他のパース
            throw new IllegalArgumentException("Uri read only :: " + nativeUri);
        }
    }

    /**
     * プラットフォーム固有のURIにパースする
     * @param nativeUri
     * @return
     */
    public static Uri parse(String nativeUri) {
        final String scheme = getScheme(nativeUri);
        final String path = getPath(nativeUri);

        if (AndroidUtil.isEmpty(scheme) || AndroidUtil.isEmpty(path)) {
            return null;
        }

        if (UriProtocol.SCHEME_APPLI_ASSETS.equals(scheme)) {
            // アセット生成
            String assetPath = String.format("file:///android_asset/" + path);
            return Uri.parse(assetPath);
        } else if (UriProtocol.SCHEME_EXTERNALSTRAGE.equals(scheme)) {
            // 外部ストレージ
            final File fullPath = buildPath(Environment.getExternalStorageDirectory(), path);
            return Uri.fromFile(fullPath);
        } else if (UriProtocol.SCHEME_LOCALSTRAGE.equals(scheme)) {
            // ローカルストレージ
            final Context context = NativeContext.getApplicationContext();
            final File fullPath = buildPath(context.getFilesDir(), path);
            return Uri.fromFile(fullPath);
        } else {
            // その他のパース
            return Uri.parse(nativeUri);
        }
    }
}
