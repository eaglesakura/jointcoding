package com.eaglesakura.tool.texpack.model;

import java.util.UUID;

import com.eaglesakura.serialize.model.Model;

public class ImageFrameModel extends Model {
    /**
     * 拡張子を含むファイル名
     */
    public String filename = null;

    /**
     * 描画時のsourceとなるXY座標+WH
     */
    public Rectangle frame = null;

    /**
     * 90度回転していたらtrue
     */
    public boolean rotated = false;

    /**
     * トリミングされていたらtrue
     */
    public boolean trimmed = false;

    /**
     * フレーム内のスプライト値
     */
    public Rectangle spriteSourceSize = null;

    /**
     * 実サイズを示す
     */
    public Size sourceSize = null;

    /**
     * 四角形を示す
     */
    public static class Rectangle extends Model {
        public int x;
        public int y;
        public int w;
        public int h;
    }

    /**
     * 四方サイズを示す
     */
    public static class Size extends Model {
        public int w;
        public int h;
    }

    /**
     * アクセス用のUUIDを生成する
     * ImageFrameの値で一意に確定する
     * @param frame
     * @return
     */
    public static String genUUID(ImageFrameModel frame) {
        String src = String.format("%s/%d,%d,%d,%d/%s/%s/%d,%d,%d,%d/%d,%d", //
                frame.filename, //
                frame.frame.x, frame.frame.y, frame.frame.w, frame.frame.h, //
                String.valueOf(frame.rotated), //
                String.valueOf(frame.trimmed), //
                frame.spriteSourceSize.x, frame.spriteSourceSize.y, frame.spriteSourceSize.w, frame.spriteSourceSize.h, //
                frame.sourceSize.w, frame.sourceSize.h //
                );

        return UUID.nameUUIDFromBytes(src.getBytes()).toString().replaceAll("-", "");
    }
}
