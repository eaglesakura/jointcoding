/*
 * ImageView.h
 *
 *  Created on: 2013/04/24
 */

#ifndef GLVIEW_IMAGEVIEW_H_
#define GLVIEW_IMAGEVIEW_H_

#include    "jc/widget/View.h"
#include    "jc/gl/texture/TextureImage.h"

namespace jc {
namespace view {

/**
 * 画像のレイアウト設定
 */
enum ImageLayout_e {
    /**
     * 画像をレイアウトの領域フルに利用する
     */
    ImageLayout_Fill,

    /**
     * 画像をレイアウト領域フルで利用するかつアスペクト比に合わせて拡縮する
     */
    ImageLayout_AspectFit,

    /**
     * オリジナルの大きさで表示する
     */
    ImageLayout_Origin,
};

/**
 * ImageViewの描画フラグ
 */
enum ImageViewFlag_e {
    /**
     * 背景として扱う。
     * 画像描画以降、アルファ値の描画をロックする
     */
    ImageViewFlag_Background,

    /**
     * 数管理用
     */
    ImageViewFlag_Num,
};

class ImageView: public View {
    /**
     * レンダリングを行う画像
     */
    MTextureImage image;

    /**
     * 管理フラグ
     */
    BitFlags<ImageViewFlag_Num> imageViewFlags;
public:
    ImageView();
    virtual ~ImageView();

    /**
     * URIを指定して貼り付ける
     */
    virtual void setImage(MDevice device, const Uri uri, const PixelFormat_e pixelFormat = PixelFormat_NativeRGBA, TextureLoadOption *option = NULL);

    /**
     * 画像を直接指定する
     */
    virtual void setImage(MTextureImage image) {
        this->image = image;
    }

    /**
     * 描画フラグを設定する
     */
    virtual void setRenderingFlag(const ImageViewFlag_e flag, const jcboolean set) {
        imageViewFlags.set(flag, set);
    }
public:
    /* layout */

    /**
     * レイアウトを更新する。
     */
    virtual void layoutFromImage(const ImageLayout_e layoutType);

    /**
     * レイアウトを更新する。
     */
    virtual void layoutFromImage(const ImageLayout_e imageLayout, const Vector2f &parentLayoutSize);

protected:
    /* override */
    /**
     * 自身のレンダリングを行う
     */
    virtual void onSelfRendering();
};

/**
 * managed
 */
typedef jc_sp<ImageView> MImageView;

}
}

#endif /* IMAGEVIEW_H_ */
