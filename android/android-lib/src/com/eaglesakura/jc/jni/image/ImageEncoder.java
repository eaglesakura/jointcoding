package com.eaglesakura.jc.jni.image;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.graphics.Bitmap;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * 画像のエンコーディングを行う
 */
@JCClass(
         cppNamespace = "ndk")
public class ImageEncoder {

    /**
     * 一時バッファ
     */
    Buffer buffer;

    /**
     * 画像の幅
     */
    int width;

    /**
     * 画像の高さ
     */
    int height;

    /**
     * エンコードを行った画像
     */
    Bitmap image;

    private ImageEncoder() {
    }

    /**
     * エンコードが完了した画像クラスを取得する
     * @return
     */
    public Bitmap getImage() {
        return image;
    }

    /**
     * 確保しているメモリを解放する
     */
    public void dispose() {
        if (image != null) {
            image.recycle();
            image = null;
        }

        buffer = null;
    }

    /**
     * バッファを生成する
     * @param bufferSize
     * @return
     */
    @JCMethod
    public Buffer alloc(int bufferSize) {
        dispose();

        buffer = ByteBuffer.allocateDirect(bufferSize).order(ByteOrder.nativeOrder());
        return buffer;
    }

    /**
     * 画像サイズを設定する
     * @param width
     * @param height
     */
    @JCMethod
    public void setImageSize(int width, int height) {
        this.width = width;
        this.height = height;
    }

    /**
     * RGBA画像としてBitmapを生成する
     */
    @JCMethod
    public void encodeRGBA() {

    }

    @JCMethod
    public static ImageEncoder createInstance() {
        return new ImageEncoder();
    }
}
