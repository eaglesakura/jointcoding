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
    virtual void setImage(const Uri uri);

    /**
     * 画像を直接指定する
     */
    virtual void setImage(MTextureImage image) {
        this->image = image;
    }
};

}
}

#endif /* IMAGEVIEW_H_ */
