package com.eaglesakura.tool.texpack;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import com.eaglesakura.common.util.FileUtil;
import com.eaglesakura.resource.texture.atlas.TextureAtlasData.AtlasGroup;
import com.eaglesakura.resource.texture.atlas.TextureAtlasData.AtlasTexture;
import com.eaglesakura.resource.texture.atlas.TextureAtlasData.Rectangle;
import com.eaglesakura.resource.texture.atlas.TextureAtlasData.Size;
import com.eaglesakura.serialize.json.JSON;
import com.eaglesakura.tool.texpack.model.ImageFrameModel;
import com.eaglesakura.tool.texpack.model.TexturePackerModel;

public class Main {

    private static TexturePackerModel decodePacker(File json) throws IOException {
        FileInputStream is = null;
        try {
            is = new FileInputStream(json);

            return JSON.decode(is, TexturePackerModel.class);
        } finally {
            if (is != null) {
                is.close();
            }
        }
    }

    static Rectangle newRectangle(int x, int y, int w, int h) {
        return Rectangle.newBuilder().setX(x).setY(y).setW(w).setH(h).build();
    }

    static Size newSize(int w, int h) {
        return Size.newBuilder().setW(w).setH(h).build();
    }

    /**
     * 画像範囲を生成する
     * @param model
     * @return
     */
    private static AtlasTexture buildAtlasImage(ImageFrameModel model) {
        AtlasTexture.Builder builder = AtlasTexture.newBuilder();
        builder.setUniqueId(ImageFrameModel.genUUID(model));
        builder.setFilename(FileUtil.getFileName(model.filename));
        builder.setFrame(newRectangle(model.frame.x, model.frame.y, model.frame.w, model.frame.h));
        builder.setRotated(model.rotated);
        builder.setTrimmed(model.trimmed);
        return builder.build();
    }

    private static AtlasGroup buildAtlasGroup(TexturePackerModel packerModel) {
        // protobufへシリアライズする
        AtlasGroup.Builder groupBuilder = AtlasGroup.newBuilder();
        groupBuilder.setUniqueId(TexturePackerModel.genUUID(packerModel));
        groupBuilder.setScale(packerModel.meta.scale);
        groupBuilder.setFilename(FileUtil.getFileName(packerModel.meta.image));

        // 画像フォーマットを指定
        {
            groupBuilder.setImportPixelFormat(packerModel.meta.format);
        }

        for (ImageFrameModel frame : packerModel.frames) {
            groupBuilder.addImages(buildAtlasImage(frame));
        }

        return groupBuilder.build();
    }

    public static void main(String[] args) throws Exception {

        System.out.println(String.format("%s", new File(".").getAbsolutePath()));

        // JSONからデコードする
        File file = new File("test.json");
        TexturePackerModel packerModel = decodePacker(file);

        AtlasGroup group = buildAtlasGroup(packerModel);

        File pbFile = new File("atlas.pb");
        byte[] serialized = group.toByteArray();
        {
            FileOutputStream os = new FileOutputStream(pbFile);
            os.write(serialized);
            os.flush();
            os.close();
        }
        {
            AtlasGroup deserialized = AtlasGroup.parseFrom(serialized);
            System.out.println(String.format("%s", deserialized.getFilename()));
            System.out.println(String.format("Images Num :: %d", deserialized.getImagesCount()));
        }
    }
}
