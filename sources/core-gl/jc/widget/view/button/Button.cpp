/*
 * Button.cpp
 *
 *  Created on: 2013/04/24
 */

#include    "jc/widget/view/button/Button.h"
#include    "jc/widget/layout/LayoutParams.h"
#include    "jc/graphics/RenderingFitter.h"

namespace jc {
namespace view {

Button::Button(MWindowContext context) :
        TextView(context) {
    // テキストはRead-Onlyのため、アクションは起こせない
    setFocusable(jctrue);
    setTouchable(jctrue);
    setFocusableFromTouch(jcfalse);

    setTextGravity(LayoutGravity_Center);
}

Button::~Button() {

}

/**
 * URIを指定してボタン画像を設定する
 */
void Button::setImage(const ButtonImageType_e type, MDevice device, const Uri &uri, const PixelFormat_e pixelFormat, TextureLoadOption *option) {
    MTextureImage texture = TextureImage::decode(device, uri, pixelFormat, option);
    assert(texture);
    setImage(type, texture);
}

/**
 * テクスチャを指定してボタン画像を設定する
 */
void Button::setImage(const ButtonImageType_e type, const MTextureImage texture) {
    images[type] = texture;
}

/**
 * コンテンツと同サイズにレイアウトし直す。
 */
void Button::layoutWrap(const float marginX, const float marginY) {
    Vector2f reqSize = getFontAreaSize();
    reqSize.x += (marginX * 2);
    reqSize.y += (marginY * 2);

    if (images[ButtonImageType_Normal]) {
        MTextureImage image = images[ButtonImageType_Normal];
        reqSize.x = jc::max<float>(reqSize.x, image->getWidth() + (marginX * 2));
        reqSize.y = jc::max<float>(reqSize.y, image->getHeight() + (marginY * 2));
    }

    layoutDirect(createRectFromXYWH(localArea.left, localArea.top, reqSize.x, reqSize.y));
}

/* layout */
/**
 * 親レイアウトの位置を元に設定する
 */
void Button::layoutFromImage(const ButtonLayout_e layoutType) {
    assert(isRegisteredWindow());

    View *parentView = getParentTo<View>();
    assert(parentView);
    const RectF parentLocalLayout = parentView->getLocalLayoutArea();
    layoutFromImage(layoutType, parentLocalLayout.wh());
}

/**
 * 親レイアウトの位置を元に設定する
 */
void Button::layoutFromImage(const ButtonLayout_e layoutType, const Vector2f &parentLayoutSize) {
    MTextureImage image = images[ButtonImageType_Normal];

    if (!image) {
        layoutFillParent(parentLayoutSize);
        return;
    }

    switch (layoutType) {
        case ButtonLayout_Fill: {
            layoutFillParent(parentLayoutSize);
        }
            break;
        case ButtonLayout_AspectFit: {
            RenderingFitter fitter;
            fitter.setViewport(0.0f, 0.0f, parentLayoutSize.x, parentLayoutSize.y);
            fitter.setImageAspect(image->getWidth(), image->getHeight());
            fitter.getDefaultRenderingArea(RenderingFitter::Fittype_Long, &localArea);
        }
            break;
        case ButtonLayout_Origin: {
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
 * レンダリングを行う
 */
void Button::onSelfRendering() {

    // rendering image
    const RectF area = getGlobalLayoutArea();
    if (images[ButtonImageType_Normal]) {
        MSpriteManager spriteManager = getSpriteManager();
        // 通常画像
        spriteManager->renderingImage(images[ButtonImageType_Normal], area, Color::fromRGBAf(1, 1, 1, getVisibleWeight()));

        // focus / down
        {
            MTextureImage imgFocus = images[ButtonImageType_Focus];
            MTextureImage imgDown = images[ButtonImageType_Down];

            if (!imgFocus) {
                imgFocus = images[ButtonImageType_Normal];
            }

            if (!imgDown) {
                imgDown = images[ButtonImageType_Normal];
            }

            spriteManager->renderingImage(imgFocus, area, Color::fromRGBAf(1, 1, 1, jc::max<float>(getDownWeight(), getFocusWeight())));
            spriteManager->renderingImage(imgDown, area, Color::fromRGBAf(1, 1, 1, getDownWeight()));
        }

    }

    TextView::onSelfRendering();
}

}
}
