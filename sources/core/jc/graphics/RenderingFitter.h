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
    RectF viewport;

    /**
     * 画像レンダリングを行うエリア
     */
    RectF image;

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

    enum FitType_e {
        /**
         * 長辺がフィット（画面全体に収まる）ようにする。
         */
        Fittype_Long,
    };
private:
    FitType_e fitting;
public:
    RenderingFitter() {
        fitting = Fittype_Long;
        aspect = 1.0f;
        defPixels = 1.0f;

        viewport.setXYWH(0, 0, 1, 1);
        image.setXYWH(0, 0, 1, 1);
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
    void setViewport(const int x, const int y, const int width, const int height) {
        viewport.setXYWH(x, y, width, height);
    }

    /**
     * レンダリング範囲を設定する。
     * @param x
     * @param y
     * @param width
     * @param height
     */
    void setViewport(const float x, const float y, const float width, const float height) {
        viewport.setXYWH(x, y, width, height);
    }

    /**
     * レンダリング範囲を設定する。
     * @param x
     * @param y
     * @param width
     * @param height
     */
    void setViewport(const RectF vp) {
        viewport = vp;
    }

    /**
     * 画像の幅・高さから画像のアスペクトを設定する。
     * @param width
     * @param height
     */
    void setImageAspect(const s32 width, const s32 height) {
        if (height && width) {
            aspect = (float) width / (float) height;
        } else {
            aspect = 1;
        }
        setDefaultFitting(Fittype_Long);
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
    void setDefaultFitting(FitType_e fitting) {
        this->fitting = fitting;
        getDefaultRenderingArea(fitting, &image);
        defPixels = image.width() * image.height();
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
    jcboolean isXLongViewport() {
        return viewport.width() > viewport.height();
    }

    /**
     * Y方向に長いエリア
     * @return
     */
    jcboolean isYLongViewport() const {
        return viewport.height() >= viewport.width();
    }

    float getViewportWidth() {
        return viewport.width();
    }

    float getViewportHeight() {
        return viewport.height();
    }

    /**
     * デフォルトのレンダリングエリアを取得する。
     * @param fitting
     * @param result
     * @return
     */
    RectI* getDefaultRenderingArea(const FitType_e fitting, RectI *result) const {
        RectF area;
        getDefaultRenderingArea(fitting, &area);

        result->left = (s32) area.left;
        result->top = (s32) area.top;
        result->right = (s32) area.right;
        result->bottom = (s32) area.bottom;

        return result;
    }

    /**
     * デフォルトのレンダリングエリアを取得する。
     * @param fitting
     * @param result
     * @return
     */
    RectF* getDefaultRenderingArea(const FitType_e fitting, RectF *result) const {
        double width = 0;
        double height = 0;
        switch (fitting) {
            case Fittype_Long:
                if (isYLongViewport()) {
                    width = viewport.width();
                    height = width / aspect;

                    const double mul = (double) height / (double) viewport.height();
                    if (mul > 1) {
                        width /= mul;
                        height /= mul;
                    }

                } else {
                    height = viewport.height();
                    width = height * aspect;

                    const double mul = (double) width / (double) viewport.width();
                    if (mul > 1) {
                        width /= mul;
                        height /= mul;
                    }

                }
                break;
            default:
                break;
        }

        result->setXYWH(0, 0, jc::round(width), jc::round(height));
        result->offset(jc::round(viewport.centerX() - result->centerX()), jc::round(viewport.centerY() - result->centerY()));

        // 誤差を修正する
        if (jc::abs<float>(result->width() - viewport.width()) <= 2) {
            result->left = viewport.left;
            result->right = viewport.right;
        }
        if (jc::abs<float>(result->height() - viewport.height()) <= 2) {
            result->top = viewport.top;
            result->bottom = viewport.bottom;
        }

        return result;
    }

    /**
     * 画像エリアを取得する。
     * @param result
     * @return
     */
    RectF* getImage(RectF *result) const {
        *result = image;
        return result;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageLeft() const {
        return image.left;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageTop() const {
        return image.top;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageRight() const {
        return image.right;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageBottom() const {
        return image.bottom;
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageWidth() const {
        return image.width();
    }

    /**
     * 実際に画像を描画するエリア取得
     * @return
     */
    float getImageHeight() const {
        return image.height();
    }

    /**
     * 画像エリアを取得する。
     * @param result
     * @return
     */
    RectI* getImage(RectI *result) const {
        result->left = (int) image.left;
        result->right = (int) image.right;
        result->top = (int) image.top;
        result->bottom = (int) image.bottom;
        return result;
    }

    /**
     * ピクセル座標からU座標に変換する。
     * @param x
     * @return
     */
    float pixToImageU(float x) const {
        const float offset = x - image.left;
        return offset / image.width();
    }

    float uToImagePix(float u) const {
        u *= image.width();
        u += image.left;

        return u;
    }

    /**
     * ピクセル座標からV座標に変換する。
     * @param y
     * @return
     */
    float pixToImageV(float y) const {
        const float offset = y - image.top;
        return offset / image.height();
    }

    float vToImagePix(float v) const {
        v *= image.height();
        v += image.top;

        return v;
    }

    /**
     * イメージエリアの位置をオフセットさせる。
     * @param x
     * @param y
     */
    void offset(float x, float y) {
        image.offset(x, y);
    }

    /**
     * レンダリングエリアとジェスチャエリアの中心が同じならtrueを返す。
     * @return
     */
    jcboolean isCenterFitting() const {
        return image.centerX() == viewport.centerX() && image.centerY() == viewport.centerY();
    }

    /**
     * ピクセル数ベースで拡大率を取得する。
     * @return
     */
    float getPixelScale() const {
        return (image.width() * image.height()) / defPixels;
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
    void correctImageArea(const jcboolean xCrrect, const jcboolean yCorrect) {

        if (xCrrect) {
            if (image.width() < viewport.width()) {
                image.offset(viewport.centerX() - image.centerX(), 0);
            } else {
                if (image.left > viewport.left) {
                    image.offset(viewport.left - image.left, 0);
                } else if (image.right < viewport.right) {
                    image.offset(viewport.right - image.right, 0);
                }
            }
        }

        if (yCorrect) {
            if (image.height() < viewport.height()) {
                image.offset(0, viewport.centerY() - image.centerY());
            } else {
                if (image.top > viewport.top) {
                    image.offset(0, viewport.top - image.top);
                } else if (image.bottom < viewport.bottom) {
                    image.offset(0, viewport.bottom - image.bottom);
                }
            }
        }
    }

    // viewport系

    /**
     * レンダリング領域を取得する。
     * @param result
     * @return
     */
    RectF* getViewport(RectF *result) const {
        *result = viewport;
        return result;
    }

    /**
     * レンダリング領域を取得する
     * @param result
     * @return
     */
    RectI* getViewport(RectI *result) const {

        result->left = (int) viewport.left;
        result->right = (int) viewport.right;
        result->top = (int) viewport.top;
        result->bottom = (int) viewport.bottom;

        return result;
    }

    float getViewportLeft() const {
        return viewport.left;
    }

    float getViewportTop() const {
        return viewport.top;
    }

    float getViewportRight() const {
        return viewport.right;
    }

    float getViewportBottom() const {
        return viewport.bottom;
    }

    float getViewoprtCenterX() const {
        return viewport.centerX();
    }

    float getViewportCenterY() const {
        return viewport.centerY();
    }
};

}

#endif /* RENDERINGFITTER_H_ */
