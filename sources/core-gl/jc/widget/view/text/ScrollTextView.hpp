/*
 * ScrollTextView.h
 *
 *  Created on: 2013/05/07
 */

#ifndef SCROLLTEXTVIEW_H_
#define SCROLLTEXTVIEW_H_

#include    "jc/widget/view/text/TextView.h"

namespace jc {
namespace view {

/**
 * スクロールを行うTextView
 */
class ScrollTextView: public TextView {
    /**
     * 毎秒のスクロール速度
     */
    float scrollSpeedPixelParSec;

    /**
     * ヘッダでスクロールを止める秒数
     */
    float headerStopSec;

    /**
     * フッダでスクロールを止める秒数
     */
    float fooderStopSec;

    /**
     * フォントのView左端からのオフセット値
     */
    float fontOffset;

    enum {
        /**
         * 文字列左端とViewの右端が一致している状態
         */
        State_HeaderStop,

        /**
         * 左の画面外に移動中
         */
        State_MoveToLeftOut,

        /**
         * 文字列右端とViewの右端が一致している状態
         */
        State_FooderStop,

        /**
         * 文字列の左端とViewの右端が一致するように移動する
         */
        State_MoveToLeftHeader,
    } state;

    /**
     * ステートの時間を制御するタイマー
     */
    WindowTimer stateTimer;

public:
    ScrollTextView() {
        fontOffset = 0;
        scrollSpeedPixelParSec = 15;
        headerStopSec = 5;
        fooderStopSec = 1;
        state = State_HeaderStop;
    }

    virtual ~ScrollTextView() {
    }

    /**
     * Viewの左端に達してから停止している秒数
     */
    virtual void setHeaderStopSec(const float headerStopSec) {
        this->headerStopSec = headerStopSec;
    }

    /**
     * 毎秒のスクロール速度を設定する
     */
    virtual void setScrollPixelsParSec(const float pixelsParSec) {
        this->scrollSpeedPixelParSec = pixelsParSec;
    }

    /**
     * Viewの左端にテキストの右端が達してから停止する秒数
     */
    virtual void setFooderStopSec(const float fooderStopSec) {
        this->fooderStopSec = fooderStopSec;
    }

protected:
    virtual void onRegisteredWindow() {
        TextView::onRegisteredWindow();

        stateTimer.initialize(windowContext, headerStopSec);
    }

    virtual MFontTexture getFont() const {
        return *fontTextures.begin();
    }

    /**
     * フォントスクロールの必要があればtrue
     */
    virtual jcboolean isFontWidthOver() const {
        return getFont()->getWidth() > getLocalLayoutSize().x;
    }

    /**
     * テキストを左へスクロールする
     */
    virtual void scrollLeft() {
        const float movePixel = windowContext->frameValue<float>(scrollSpeedPixelParSec);

        // フォントを左へ移動する
        fontOffset -= movePixel;
    }

    /**
     * 左の画面外に出たらtrue
     */
    virtual jcboolean isLeftOut() const {
        MFontTexture font = getFont();
        return jc::abs<float>(fontOffset) > font->getWidth();
    }

    virtual void updateHeaderStop() {
        if (stateTimer.overWithReset()) {
            state = State_MoveToLeftOut;
        }
    }

    virtual void updateMoveToLeftOut() {
        scrollLeft();
        if (isLeftOut()) {
            state = State_FooderStop;
            stateTimer.setTimerSec(fooderStopSec);
            stateTimer.reset();
        }
    }

    virtual void updateFooderStop() {
        if (stateTimer.overWithReset()) {
            // フォント位置を左端に移動する
            fontOffset = getLocalLayoutSize().x + 5;
            state = State_MoveToLeftHeader;
        }
    }

    virtual void updateMoveToHeader() {
        scrollLeft();
        if (fontOffset < 0) {
            // 左端をオーバーしたら調整して停止
            fontOffset = 0;
            state = State_HeaderStop;
            stateTimer.setTimerMS(headerStopSec);
            stateTimer.reset();
        }
    }

    virtual jcboolean onSelfUpdate() {
        // フォント幅がスクロール以上であれば
        if (textureCreated && isFontWidthOver()) {
            switch (state) {
                case State_HeaderStop:
                    updateHeaderStop();
                    break;
                case State_MoveToLeftOut:
                    updateMoveToLeftOut();
                    break;
                case State_FooderStop:
                    updateFooderStop();
                    break;
                case State_MoveToLeftHeader:
                    updateMoveToHeader();
                    break;
                default:
                    assert(false);
                    break;
            }
        }

        return TextView::onSelfUpdate();
    }

    /**
     * レンダリングを行う
     */
    virtual void onSelfRendering() {
        if (!textureCreated) {
            return;
        }

        // 文字列は１行でなければならない
        assert(fontTextures.size() == 1);

        MSpriteManager spriteManager = getSpriteManager();

        // テキスト全体の描画エリア
        const RectF textArea = getFontRendringArea();

        // 描画エリアロック
        spriteManager->pushRenderArea();
        spriteManager->setRenderArea(getGlobalLayoutArea());
#if 0
        // エリアをデバッグ描画
        spriteManager->renderingArea(textArea, 0xFF00007F, 0x0000FFFF);
        spriteManager->renderingArea(getGlobalLayoutArea(), 0x00FF007F, 0xFF00FFFF);
#endif

        const float y = textArea.top;
        const float AREA_WIDTH = textArea.width();

        // 全テクスチャの描画
        MFontTexture font = (*fontTextures.begin());
        const Color color = getTextColorWithVisible();
        {
            // 左寄せ描画
            spriteManager->renderingImage(font, textArea.left + fontOffset, y, color);
        }

        // 描画エリアアンロック
        spriteManager->popRenderArea();
    }
};

/**
 * managed
 */
typedef jc_sp<ScrollTextView> MScrollTextView;

}
}

#endif /* SCROLLTEXTVIEW_H_ */
