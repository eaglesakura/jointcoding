package com.eaglesakura.jc.android.app.font;

import java.util.ArrayList;
import java.util.List;

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

        final String[] lines = text.split("\n");
        {
            String heightCheckText = "";
            {
                for (String txt : lines) {
                    heightCheckText += txt;
                }
            }
            Point size = new Point();
            int fontSize = heightPixel + 5;
            while (calcTextSize(heightCheckText, fontSize, size).y > heightPixel && heightPixel > 1) {
                //                AndroidUtil.log(String.format("TextSize(%d x %d)", size.x, size.y));
                --fontSize;
            }
        }
        paint.setAntiAlias(true);

        // 複数行テキストをチェック
        {
            Rect temp = new Rect();
            for (String singleLine : lines) {
                if (bounds.bottom == 0) {
                    paint.getTextBounds(singleLine, 0, singleLine.length(), bounds);
                } else {
                    paint.getTextBounds(singleLine, 0, singleLine.length(), temp);
                    bounds.bottom += temp.height();
                    bounds.right = Math.max(bounds.right, temp.right);
                }
            }
        }

        FontMetrics fontMetrics = paint.getFontMetrics();
        final int IMAGE_WIDTH = Math.max(1, bounds.width());
        final int IMAGE_HEIGHT = (int) Math.max(//
                Math.abs(fontMetrics.ascent) + Math.abs(fontMetrics.descent),
                //
                (Math.abs(fontMetrics.top) + Math.abs(fontMetrics.bottom)));

        image = Bitmap.createBitmap(IMAGE_WIDTH, IMAGE_HEIGHT * lines.length, Config.ARGB_8888);

        Canvas canvas = new Canvas(image);
        paint.setColor(0xffffffff);

        // 複数行テキストをレンダリング
        {
            int y = 0;
            for (String singleLine : lines) {
                paint.getTextBounds(singleLine, 0, singleLine.length(), bounds);
                canvas.drawText(singleLine, -bounds.left, y - fontMetrics.top, paint);
                y += IMAGE_HEIGHT;
            }
        }
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
    public synchronized int calcFontSize(final String text, int heightPixel) {
        Point size = new Point();
        int fontSize = heightPixel + 5;
        while (calcTextSize(text, fontSize, size).y > heightPixel && heightPixel > 1) {
            //                AndroidUtil.log(String.format("TextSize(%d x %d)", size.x, size.y));
            --fontSize;
        }
        return fontSize;
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
     * 特定サイズに収めることができる文字を生成する。
     * height/widthの値は変更せず、オーバーする場合はテキストの末尾を削ってfooderTextに置き換える。
     * @param baseText
     * @param fooderText
     * @param heightPixel 1行の高さ
     * @param widthPixel
     * @return
     */
    public String calcTextInRect(final String baseText, final String fooderText, int widthPixel, int heightPixel) {
        final int fontSize = calcFontSize(baseText, heightPixel);
        String tempText = baseText;
        Point tempTextSize = new Point();

        int index = baseText.length();
        while (tempText.length() > 0 && calcTextSize(tempText, fontSize, tempTextSize).x > widthPixel) {
            tempText = baseText.substring(0, --index) + fooderText;
        }

        return tempText;
    }

    /**
     * 特定サイズに収めることができる文字を生成する。
     * height/widthの値は変更せず、オーバーする場合は折り返しを行う
     * @param baseText
     * @param fooderText
     * @param heightPixel 1行の高さ
     * @param widthPixel
     * @return
     */
    public String calcTextInRectNewLine(final String baseText, int widthPixel, int heightPixel) {
        final int fontSize = calcFontSize(baseText, heightPixel);
        String allText = baseText;
        Point tempTextSize = new Point();

        List<String> result = new ArrayList<String>();

        while (!allText.isEmpty()) {
            int index = allText.length();
            String tempText = allText;
            while (tempText.length() > 0 && calcTextSize(tempText, fontSize, tempTextSize).x > widthPixel) {
                tempText = allText.substring(0, --index);
            }

            result.add(tempText);
            allText = allText.substring(tempText.length());
        }

        String tempResult = "";
        int index = 0;
        for (String txt : result) {
            if (index > 0) {
                tempResult += "\n";
            }
            tempResult += txt;
            ++index;
        }

        return tempResult;
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
