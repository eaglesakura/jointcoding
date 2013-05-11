package com.eaglesakura.jc.jni.image;

import java.io.InputStream;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.eaglesakura.jc.util.AndroidUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * 画像のデコードを行う。
 * Androidが実装するデコーダーを利用する。
 * 
 * メモリ効率・速度的には余り高くないが、確実にデコードを行える。
 */
@JCClass(
         cppNamespace = "ndk")
public class ImageDecoder {
    /**
     * 幅
     */
    int width = 0;

    /**
     * 高さ
     */
    int height = 0;

    /**
     * ピクセルデータ
     */
    Buffer pixels;

    private ImageDecoder() {
    }

    /**
     * 画像幅を取得する
     * @return
     */
    @JCMethod
    public int getWidth() {
        return width;
    }

    /**
     * 画像高を取得する
     * @return
     */
    @JCMethod
    public int getHeight() {
        return height;
    }

    /**
     * ピクセルバッファを取得する
     * @return
     */
    @JCMethod
    public Buffer getPixels() {
        return pixels;
    }

    /**
     * 画像からデコードを行う
     * 画像は自動ではrecycleされないため、呼び出し元で行うこと。
     * @param image
     * @return
     */
    @JCMethod
    public static ImageDecoder decodeFromBitmap(Bitmap image, boolean recycle) {
        if (image == null) {
            return null;
        }

        final int image_width = image.getWidth();
        final int image_height = image.getHeight();
        ImageDecoder result = new ImageDecoder();

        // ピクセル情報の格納先を確保
        IntBuffer pixelBuffer = ByteBuffer.allocateDirect(image_width * image_height * 4)
                .order(ByteOrder.LITTLE_ENDIAN).asIntBuffer();
        {
            result.width = image_width;
            result.height = image_height;
            result.pixels = pixelBuffer;
        }

        AndroidUtil.log(String.format("image size(%d x %d)", image_width, image_height));

        final int ONCE_READ_LINE = 6;
        final int[] temp = new int[image_width * ONCE_READ_LINE];

        int readHeight = image_height;
        while (readHeight > 0) {
            // 1ラインずつ読み込む
            final int readLine = Math.min(readHeight, ONCE_READ_LINE);
            image.getPixels(temp, 0, image_width, 0, image_height - readHeight, image_width, readLine);
            pixelBuffer.put(temp, 0, image_width * readLine);

            readHeight -= readLine;
        }

        // 書き込み位置をリセットする
        pixelBuffer.position(0);

        if (recycle) {
            image.recycle();
        }
        return result;
    }

    /**
     * 画像をデコードする。
     * streamは自動では閉じないため、呼び出し元で閉じること。
     * @param stream
     * @return
     */
    @JCMethod
    public static ImageDecoder decodeFromStream(InputStream stream) {
        try {
            Bitmap image = BitmapFactory.decodeStream(stream);
            return decodeFromBitmap(image, true);
        } catch (Exception e) {
            e.printStackTrace();
        }

        return null;
    }
}
