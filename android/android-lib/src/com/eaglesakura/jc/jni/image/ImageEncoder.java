package com.eaglesakura.jc.jni.image;

import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Color;

import com.eaglesakura.jc.util.AndroidUtil;
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
    ByteBuffer buffer;

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
    public boolean encodeRGBA() {
        try {
            if (buffer == null) {
                AndroidUtil.log("buffer is null...");
                return false;
            }

            if (width <= 0 || height <= 0) {
                AndroidUtil.log("bitmap size error");
                return false;
            }

            image = Bitmap.createBitmap(width, height, Config.ARGB_8888);

            IntBuffer temp = buffer.asIntBuffer();
            int[] pixels = new int[width];
            for (int y = 0; y < height; ++y) {
                // nativeからピクセルをキャプチャする
                temp.position(y * width);
                temp.get(pixels, 0, pixels.length);

                // ビットの並びを変換する
                for (int i = 0; i < pixels.length; ++i) {
                    final int color = pixels[i];
                    final int a = (color >> 24) & 0xFF;
                    final int b = (color >> 16) & 0xFF;
                    final int g = (color >> 8) & 0xFF;
                    final int r = (color >> 0) & 0xFF;
                    pixels[i] = Color.argb(a, r, g, b);
                }

                // キャプチャしたピクセルをBitmapへ書き込む
                image.setPixels(pixels, 0, width, 0, y, width, 1);
            }

            return true;
        } catch (Exception e) {
            AndroidUtil.log(e);
            return false;
        }
    }

    @JCMethod
    public static ImageEncoder createInstance() {
        return new ImageEncoder();
    }
}
