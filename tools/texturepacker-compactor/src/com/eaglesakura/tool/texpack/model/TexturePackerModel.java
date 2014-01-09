package com.eaglesakura.tool.texpack.model;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

import com.eaglesakura.resource.texture.Texture;
import com.eaglesakura.serialize.model.Model;

public class TexturePackerModel extends Model {

    /**
     * 
     */
    public List<ImageFrameModel> frames = new ArrayList<ImageFrameModel>();

    /**
     * 
     */
    public Meta meta;

    public static class Meta extends Model {
        /**
         * ファイルバージョン
         */
        public String version;

        /**
         * 出力したファイル名
         */
        public String image;

        /**
         * フォーマット
         */
        public Texture.PixelFormat format;

        /**
         * スケーリング値
         */
        public float scale;
    }

    public static String genUUID(TexturePackerModel model) {
        return UUID.nameUUIDFromBytes(model.meta.version.getBytes()).toString().replaceAll("-", "");
    }
}
