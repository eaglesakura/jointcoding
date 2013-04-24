/*
 * ImageView.cpp
 *
 *  Created on: 2013/04/24
 */

#include    "jc/widget/view/image/ImageView.h"
#include    "jc/widget/layout/LayoutParams.h"
#include    "jc/graphics/RenderingFitter.h"

namespace jc {
namespace view {

ImageView::ImageView() {

}

ImageView::~ImageView() {

}

/**
 * URIを指定して貼り付ける
 */
void ImageView::setImage(MDevice device, const Uri uri, const PixelFormat_e pixelFormat, TextureLoadOption *option) {
    MTextureImage texture = TextureImage::decode(device, uri, pixelFormat, option);
    assert(texture);
    setImage(texture);
}

/**
 * レイアウトを更新する。
 */
void ImageView::layout(const ImageLayout_e imageLayout) {
    assert(isRegisteredWindow());

    View *parentView = getParentTo<View>();
    assert(parentView);
    const RectF parentLocalLayout = parentView->getLocalLayoutArea();
    layout(imageLayout, parentLocalLayout.wh());
}

/**
 * レイアウトを更新する。
 */
void ImageView::layout(const ImageLayout_e imageLayout, const Vector2f &parentLayoutSize) {
    if (!image) {
        layoutFillParent(parentLayoutSize);
        return;
    }

    switch (imageLayout) {
        case ImageLayout_Fill: {
            layoutFillParent(parentLayoutSize);
        }
            break;
        case ImageLayout_AspectFit: {
            RenderingFitter fitter;
            fitter.setViewport(0.0f, 0.0f, parentLayoutSize.x, parentLayoutSize.y);
            fitter.setImageAspect(image->getWidth(), image->getHeight());
            fitter.getDefaultRenderingArea(RenderingFitter::Fittype_Long, &localArea);
        }
            break;
        case ImageLayout_Origin: {
            LayoutParams params;
            params.layout_width = image->getWidth();
            params.layout_height = image->getHeight();
            params.gravity = LayoutGravity_Center;
            params.layout(&localArea, parentLayoutSize);
        }
            break;
    }
}


/**
 * 自身のレンダリングを行う
 */
void ImageView::onSelfRendering() {
    if (!image) {
        return;
    }

    const Color color = Color::fromRGBAf(1, 1, 1, getVisibleWeight());
    getSpriteManager()->renderingImage(image, getGlobalLayoutArea(), 0, color.rgba);
}

}
}

