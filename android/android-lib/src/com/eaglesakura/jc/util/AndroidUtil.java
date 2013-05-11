package com.eaglesakura.jc.util;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import android.os.Looper;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         cppNamespace = "ndk")
public class AndroidUtil {
    /**
     * 単純にsleepさせる。
     * 
     * @param timems
     */
    public static void sleep(long timems) {
        try {
            Thread.sleep(timems);
        } catch (Exception e) {
        }
    }

    /**
     * ナノ秒単位でスリープする
     * @param millisec
     * @param nanosec
     */
    public static void sleep(long millisec, int nanosec) {
        try {
            Thread.sleep(millisec, nanosec);
        } catch (Exception e) {
        }
    }

    @JCMethod(
              nativeMethod = true)
    static native void _log(String message);

    public static void log(String message) {
        //        _log(message);
        Log.i("JC-LOG", message);
    }

    public static void log(Exception e) {
        //        _log(message);
        if (e == null) {
            return;
        }
        e.printStackTrace();
        Log.i("JC-LOG", "" + e.getMessage());
    }

    /**
     * サーフェイスホルダからサーフェイス本体を取得する
     * @param holder
     * @return
     */
    @JCMethod
    public static Surface getSurface(SurfaceHolder holder) {
        return holder.getSurface();
    }

    /**
     * UIスレッドだったらtrueを返す。
     * 
     * @return
     */
    public static boolean isUIThread() {
        return Thread.currentThread().equals(Looper.getMainLooper().getThread());
    }

    /**
     * UIスレッドでなければ例外を投げる。
     */
    public static void assertUIThread() {
        if (!isUIThread()) {
            throw new IllegalStateException("is not ui thread!!");
        }
    }

    /**
     * InputStreamを全てメモリ上に展開する。 isの長さがOOMにならないように調整すること。
     * 
     * @param is
     * @return
     * @throws IOException
     */
    public static byte[] toByteArray(InputStream is) throws IOException {
        byte[] result = null;

        //! 1kbずつ読み込む。
        byte[] tempBuffer = new byte[1024 * 5];
        //! 元ストリームを読み取り
        {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            int n = 0;
            while ((n = is.read(tempBuffer)) > 0) {
                baos.write(tempBuffer, 0, n);
            }
            result = baos.toByteArray();
            is.close();
        }

        return result;
    }

    /**
     * InputStreamを全てメモリ上に展開する。 isの長さがOOMにならないように調整すること。
     * 
     * @param is
     * @param close
     * @return
     * @throws IOException
     */
    public static byte[] toByteArray(InputStream is, boolean close) throws IOException {
        byte[] result = null;

        //! 1kbずつ読み込む。
        byte[] tempBuffer = new byte[1024 * 5];
        //! 元ストリームを読み取り
        {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            int n = 0;
            while ((n = is.read(tempBuffer)) > 0) {
                baos.write(tempBuffer, 0, n);
            }
            result = baos.toByteArray();
            if (close) {
                is.close();
            }
        }

        return result;
    }

    /**
     * inputのバッファを全てoutputへコピーする。 完了した時点でストリームはcloseされる。
     * 
     * @param input
     * @param output
     * @throws IOException
     */
    public static void copyTo(InputStream input, OutputStream output) throws IOException {
        byte[] buffer = new byte[1024 * 128];
        int length = 0;

        while ((length = input.read(buffer)) > 0) {
            output.write(buffer, 0, length);
        }

        input.close();
        output.close();
    }

    /**
     * inputのバッファを全てoutputへコピーする。 
     * close=trueの場合、完了した時点でストリームはcloseされる。
     * 
     * @param input
     * @param output
     * @throws IOException
     */
    public static void copyTo(InputStream input, OutputStream output, boolean close) throws IOException {
        byte[] buffer = new byte[1024 * 128];
        int length = 0;

        while ((length = input.read(buffer)) > 0) {
            output.write(buffer, 0, length);
        }

        if (close) {
            input.close();
            output.close();
        }
    }

    /**
     * min <= result <= maxとなるようにnowを補正する。
     * 
     * 
     * @param min
     * @param max
     * @param now
     * @return
     */
    public static final int minmax(int min, int max, int now) {
        if (now < min)
            return min;
        if (now > max)
            return max;
        return now;
    }

    /**
     * min <= result <= maxとなるようにnowを補正する。
     * 
     * 
     * @param min
     * @param max
     * @param now
     * @return
     */
    public static final float minmax(float min, float max, float now) {
        if (now < min)
            return min;
        if (now > max)
            return max;
        return now;
    }

    /**
     * 特定のビットフラグが立っていることを検証する。
     * 
     * 
     * @param flg
     * @param check
     * @return
     */
    public static final boolean isFlagOn(int flg, int check) {
        return (flg & check) != 0;
    }

    /**
     * 特定のビットフラグがすべて立っていることを検証する。
     * 
     * 
     * @param flg
     * @param check
     * @return
     */
    public static final boolean isFlagOnAll(int flg, int check) {
        return (flg & check) == 0;
    }

    /**
     * フラグ情報を設定する。
     * 
     * 
     * @param flg
     * @param check
     * @param is
     *            ビットを立てる場合はtrue、下げる場合はfalse
     * @return
     */
    public static final int setFlag(int flg, int check, boolean is) {
        if (is)
            return flg | check;
        else
            return flg & (~check);
    }

    /**
     * Byte配列に変換する。
     * @param array
     * @return
     */
    public static final byte[] toByteArray(int[] array) {
        byte[] result = new byte[array.length * 4];
        return toByteArray(array, result);
    }

    public static final byte[] toByteArray(int[] array, byte[] result) {
        for (int i = 0; i < array.length; ++i) {
            result[i * 4 + 0] = (byte) ((array[i] >> 24) & 0xff);
            result[i * 4 + 1] = (byte) ((array[i] >> 16) & 0xff);
            result[i * 4 + 2] = (byte) ((array[i] >> 8) & 0xff);
            result[i * 4 + 3] = (byte) ((array[i] >> 0) & 0xff);
        }
        return result;
    }

    /**
     * 文字列がnullか空文字だったらtrueを返す。
     * @param str
     * @return
     */
    public static boolean isEmpty(String str) {
        if (str == null) {
            return true;
        }

        return str.length() == 0;
    }

    /**
     * strがnullかemptyだったらnullを返す。
     * @param str
     * @return
     */
    public static String emptyToNull(String str) {
        return isEmpty(str) ? null : str;
    }
}
