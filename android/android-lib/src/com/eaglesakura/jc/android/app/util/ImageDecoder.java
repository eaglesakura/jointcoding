package com.eaglesakura.jc.android.app.util;

import java.io.InputStream;
import java.nio.Buffer;
import java.nio.ByteBuffer;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

import com.eaglesakura.jc.android.app.AndroidUtil;
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
        ByteBuffer pixelBuffer = ByteBuffer.allocateDirect(image_width * image_height * 4);
        {
            result.width = image_width;
            result.height = image_height;
            result.pixels = pixelBuffer;
        }

        AndroidUtil.log(String.format("image size(%d x %d)", image_width, image_height));

        final int[] temp = new int[image_width];
        final byte[] pixel_temp = new byte[4];
        for (int i = 0; i < image_height; ++i) {
            // 1ラインずつ読み込む
            image.getPixels(temp, 0, image_width, 0, i, image_width, 1);
            // 結果をByteArrayへ書き込む
            for (int k = 0; k < image_width; ++k) {
                final int pixel = temp[k];

                pixel_temp[0] = (byte) ((pixel >> 16) & 0xFF);
                pixel_temp[1] = (byte) ((pixel >> 8) & 0xFF);
                pixel_temp[2] = (byte) ((pixel) & 0xFF);
                pixel_temp[3] = (byte) ((pixel >> 24) & 0xFF);

                pixelBuffer.put(pixel_temp);
            }
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
