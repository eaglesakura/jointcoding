package com.eaglesakura.jc.android.app.font;

import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.FontMetrics;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.Typeface;

import com.eaglesakura.jc.android.app.util.ImageDecoder;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * アプリ内で利用するフォント
 */
@JCClass(
         cppNamespace = "ndk")
public class AppFont {

    /**
     * フォント
     */
    Typeface font;

    /**
     * レンダリング用paint
     */
    Paint paint = new Paint();

    /**
     * レンダリング対象の画像
     */
    Bitmap image;

    private AppFont(Typeface font) {
        this.font = font;

        paint.setTypeface(font);
    }

    /**
     * 
     * @param text
     * @param heightPixel
     */
    @JCMethod
    public synchronized void createImage(final String text, int heightPixel) {
        Rect bounds = new Rect();

        {
            Point size = new Point();
            int fontSize = heightPixel + 5;
            while (calcTextSize(text, fontSize, size).y > heightPixel && heightPixel > 1) {
                //                AndroidUtil.log(String.format("TextSize(%d x %d)", size.x, size.y));
                --fontSize;
            }
        }
        paint.setAntiAlias(true);
        paint.getTextBounds(text, 0, text.length(), bounds);
        FontMetrics fontMetrics = paint.getFontMetrics();

        final int IMAGE_WIDTH = Math.max(1, bounds.width());
        final int IMAGE_HEIGHT = (int) Math.max(//
                Math.abs(fontMetrics.ascent) + Math.abs(fontMetrics.descent),
                //
                (Math.abs(fontMetrics.top) + Math.abs(fontMetrics.bottom)));

        image = Bitmap.createBitmap(IMAGE_WIDTH, IMAGE_HEIGHT, Config.ARGB_8888);

        Canvas canvas = new Canvas(image);
        paint.setColor(0xffffffff);
        canvas.drawText(text, -bounds.left, -fontMetrics.top, paint);
    }

    /**
     * 画像を取得する
     * @return
     */
    @JCMethod
    public Bitmap getImage() {
        return image;
    }

    /**
     * 画像領域の計算のみを行う。
     * @param text
     * @param start
     * @param end
     * @return
     */
    public synchronized Point calcTextSize(final String text, final int fontSize, Point result) {
        Rect bounds = new Rect();
        paint.setTextSize(fontSize);
        paint.getTextBounds(text, 0, text.length(), bounds);
        FontMetrics fontMetrics = paint.getFontMetrics();

        final int IMAGE_WIDTH = Math.max(1, bounds.width());
        final int IMAGE_HEIGHT = (int) Math.max(//
                Math.abs(fontMetrics.ascent) + Math.abs(fontMetrics.descent),
                //
                (Math.abs(fontMetrics.top) + Math.abs(fontMetrics.bottom)));

        result.x = IMAGE_WIDTH;
        result.y = IMAGE_HEIGHT;
        return result;
    }

    /**
     * デフォルトで作成する
     * @return
     */
    @JCMethod
    public static AppFont createInstance() {
        return new AppFont(Typeface.DEFAULT);
    }

    /**
     * 画像として直接作成する
     * @param text
     * @param height
     * @return
     */
    @JCMethod
    public static ImageDecoder createRawImage(final String text, final int height) {
        AppFont font = createInstance();
        font.createImage(text, height);

        ImageDecoder result = ImageDecoder.decodeFromBitmap(font.getImage(), true);

        return result;
    }
}
