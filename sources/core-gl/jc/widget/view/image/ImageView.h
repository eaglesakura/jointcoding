/*
 * ImageView.h
 *
 *  Created on: 2013/04/24
 */

#ifndef IMAGEVIEW_H_
#define IMAGEVIEW_H_

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

};

class ImageView: public View {
    /**
     * レンダリングを行う画像
     */
    MTextureImage image;

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
public:
    /* layout */

    /**
     * レイアウトを更新する。
     */
    virtual void layout(const ImageLayout_e imageLayout);

    /**
     * レイアウトを更新する。
     */
    virtual void layout(const ImageLayout_e imageLayout, const Vector2f &parentLayoutSize);

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
