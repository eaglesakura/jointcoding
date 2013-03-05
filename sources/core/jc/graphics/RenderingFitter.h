/*
 * RenderingFitter.h
 *
 *  Created on: 2013/03/05
 */

#ifndef RENDERINGFITTER_H_
#define RENDERINGFITTER_H_

#include    "jointcoding.h"
#include    "jc/math/Rect.h"

namespace jc {

class RenderingFitter: public Object {

    /**
     * レンダリング可能エリア
     */
    RectF renderArea;

    /**
     * 画像レンダリングを行うエリア
     */
    RectF imageArea;

    /**
     * width / heightの値
     */
    float aspect;

    /**
     * デフォルトのピクセル数を取得する。
     * このピクセル数と現在のピクセル数で倍率を確定する。
     */
    float defPixels;

public:

    enum FitType {
        /**
         * 長辺がフィット（画面全体に収まる）ようにする。
         */
        LONG,
    };
private:
    FitType fitting;
public:
    RenderingFitter() {
        fitting = LONG;
        aspect = 1.0f;
        defPixels = 1.0f;

        renderArea.setXYWH(0, 0, 1, 1);
        imageArea.setXYWH(0, 0, 1, 1);
    }

    virtual ~RenderingFitter() {
    }

    /**
     * レンダリング範囲を設定する。
     * @param x
     * @param y
     * @param width
     * @param height
     */
    void setRenderArea(const int x, const int y, const int width, const int height) {
        renderArea.setXYWH(x, y, width, height);
    }

    /**
     * レンダリング範囲を設定する。
     * @param x
     * @param y
     * @param width
     * @param height
     */
    void setRenderArea(const float x, const float y, const float width, const float height) {
        renderArea.setXYWH(x, y, width, height);
    }

    /**
     * 画像の幅・高さから画像のアスペクトを設定する。
     * @param width
     * @param height
     */
    void setImageAspect(const s32 width, const s32 height) {
        aspect = (float) width / (float) height;
        setDefaultFitting(LONG);
    }

    /**
     * 画像のアスペクト比を取得する。
     * @return
     */
    float getImageAspect() const {
        return aspect;
    }

    /**
     * フィッティングを指定して初期化する。
     * @param fitting
     */
    void setDefaultFitting(FitType fitting) {
        this->fitting = fitting;
        getDefaultRenderArea(fitting, &imageArea);
        defPixels = imageArea.width() * imageArea.height();
    }

    /**
     * レンダリングエリアを初期化する。
     */
    void reset() {
        setDefaultFitting(fitting);
    }

    /**
     * X方向に長い画像
     * @return
     */
    jcboolean isXLongImage() {
        return aspect > 1.0f;
    }

    /**
     * Y方向に長い画像
     * @return
     */
    jcboolean isYLongImage() {
        return aspect <= 1.0f;
    }

    /**
     * X方向に長いエリア
     * @return
     */
    jcboolean isXLongArea() {
        return renderArea.width() > renderArea.height();
    }

    /**
     * Y方向に長いエリア
     * @return
     */
    jcboolean isYLongArea() const {
        return renderArea.height() >= renderArea.width();
    }

    float getRenderAreaWidth() {
        return renderArea.width();
    }

    float getRenderAreaHeight() {
        return renderArea.height();
    }

    /**
     * デフォルトのレンダリングエリアを取得する。
     * @param fitting
     * @param result
     * @return
     */
    RectF* getDefaultRenderArea(const FitType fitting, RectF *result) const {
        float width = 0;
        float height = 0;
        switch (fitting) {
            case LONG:
                if (isYLongArea()) {
                    width = renderArea.width();
                    height = width / aspect;

                    const float mul = height / renderArea.height();
                    if (mul > 1) {
                        width /= mul;
                        height /= mul;
                    }

                } else {
                    height = renderArea.height();
                    width = height * aspect;

                    const float mul = width / renderArea.width();
                    if (mul > 1) {
                        width /= mul;
                        height /= mul;
                    }

                }
                break;
            default:
                break;
        }

        result->setXYWH(0, 0, width, height);
        result->offset(renderArea.centerX() - result->centerX(), renderArea.centerY() - result->centerY());
        return result;
    }

    /**
     * 画像エリアを取得する。
     * @param result
     * @return
     */
    RectF* getImageArea(RectF *result) const {
        *result = imageArea;
        return result;
    }

    float getRenderAreaLeft() const {
        return renderArea.left;
    }

    float getRenderAreaTop() const {
        return renderArea.top;
    }

    float getRenderAreaRight() const {
        return renderArea.right;
    }

    float getRenderAreaBottom() const {
        return renderArea.bottom;
    }

    float getRenderAreaCenterX() const {
        return renderArea.centerX();
    }

    float getRenderAreaCenterY() const {
        return renderArea.centerY();
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageAreaLeft() const {
        return imageArea.left;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageAreaTop() const {
        return imageArea.top;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageAreaRight() const {
        return imageArea.right;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageAreaBottom() const {
        return imageArea.bottom;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageAreaWidth() const {
        return imageArea.width();
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageAreaHeight() const {
        return imageArea.height();
    }

    /**
     * 画像エリアを取得する。
     * @param result
     * @return
     */
    RectI* getImageArea(RectI *result) const {
        result->left = (int) imageArea.left;
        result->right = (int)imageArea.right;
        result->top = (int)imageArea.top;
        result->bottom = (int)imageArea.bottom;
        return result;
    }

    /**
     * レンダリング領域を取得する。
     * @param result
     * @return
     */
    RectF* getRenderArea(RectF *result) const {
        *result = renderArea;
        return result;
    }

    /**
     * レンダリング領域を取得する
     * @param result
     * @return
     */
    RectI* getRenderArea(RectI *result) const {

        result->left = (int) renderArea.left;
        result->right = (int)renderArea.right;
        result->top = (int)renderArea.top;
        result->bottom = (int)renderArea.bottom;

        return result;
    }

    /**
     * ピクセル座標からU座標に変換する。
     * @param x
     * @return
     */
    float pixToImageU(float x) const {
        const float offset = x - imageArea.left;
        return offset / imageArea.width();
    }

    float uToImagePix(float u) const {
        u *= imageArea.width();
        u += imageArea.left;

        return u;
    }

    /**
     * ピクセル座標からV座標に変換する。
     * @param y
     * @return
     */
    float pixToImageV(float y) const {
        const float offset = y - imageArea.top;
        return offset / imageArea.height();
    }

    float vToImagePix(float v) const {
        v *= imageArea.height();
        v += imageArea.top;

        return v;
    }

    /**
     * イメージエリアの位置をオフセットさせる。
     * @param x
     * @param y
     */
    void offset(float x, float y) {
        imageArea.offset(x, y);
    }

    /**
     * レンダリングエリアとジェスチャエリアの中心が同じならtrueを返す。
     * @return
     */
    jcboolean isCenterFitting() const {
        return imageArea.centerX() == renderArea.centerX() && imageArea.centerY() == renderArea.centerY();
    }

    /**
     * ピクセル数ベースで拡大率を取得する。
     * @return
     */
    float getPixelScale()  const{
        return (imageArea.width() * imageArea.height()) / defPixels;
    }

    /**
     * デフォルトのスケーリング値の場合true
     * @return
     */
    jcboolean isDefaultPixelScale() const {
        const float scale = getPixelScale();
        return scale >= 1.0f && scale <= 1.0001f;
    }

    /**
     * レンダリングエリアからイメージエリアが飛び出していたら、飛び出さないようにする。
     * レンダリングエリアよりイメージエリアのほうが狭かったら、中心寄せに補正する。
     */
    void correctImageArea() {
        correctImageArea(true, true);
    }

    /**
     * レンダリングエリアからイメージエリアが飛び出していたら、飛び出さないようにする。
     * レンダリングエリアよりイメージエリアのほうが狭かったら、中心寄せに補正する。
     */
    void correctImageArea(jcboolean xCrrect, jcboolean yCorrect) {

        if (xCrrect) {
            if (imageArea.width() < renderArea.width()) {
                imageArea.offset(renderArea.centerX() - imageArea.centerX(), 0);
            } else {
                if (imageArea.left > renderArea.left) {
                    imageArea.offset(renderArea.left - imageArea.left, 0);
                } else if (imageArea.right < renderArea.right) {
                    imageArea.offset(renderArea.right - imageArea.right, 0);
                }
            }
        }

        if (yCorrect) {
            if (imageArea.height() < renderArea.height()) {
                imageArea.offset(0, renderArea.centerY() - imageArea.centerY());
            } else {
                if (imageArea.top > renderArea.top) {
                    imageArea.offset(0, renderArea.top - imageArea.top);
                } else if (imageArea.bottom < renderArea.bottom) {
                    imageArea.offset(0, renderArea.bottom - imageArea.bottom);
                }
            }
        }
    }
};

}

#endif /* RENDERINGFITTER_H_ */
