/*
 * VirtualDisplay.hpp
 *
 *  Created on: 2013/06/03
 */

#ifndef VIRTUALDISPLAY_HPP_
#define VIRTUALDISPLAY_HPP_

#include    "jointcoding.h"

namespace jc {

/**
 * ディスプレイサイズに関わる補正を行う。<BR>
 * <BR>
 * 基本的に、画面はスケーリング＆センタリングされる。<BR>
 * 基本となる仮想ディスプレイサイズを指定し、そのアスペクト比を保持して実際のディスプレイサイズが決定される。<BR>
 * アスペクト比は可能な限り保持するが、端数の関係で完全一致は諦めること。<BR>
 * 実際の描画を行うための解像度は必ず偶数になる。<BR>
 * 全ての入力が偶数である場合、上下の隙間のピクセル数は必ず一致する。<BR>
 *
 */
class VirtualDisplay: public Object {
    /**
     * 実際のディスプレイサイズ
     */
    Vector2f realDisplaySize;

    /**
     * 仮想ディスプレイサイズ
     */
    Vector2f virtualDisplaySize;

    /**
     * 仮想ディスプレイの実描画位置
     */
    RectF drawingArea;

    /**
     * サイズのスケーリング値。
     */
    float scaling;

public:

    /**
     * 仮想ディスプレイのフィットタイプ。
     * @author TAKESHI YAMASHITA
     *
     */
    enum FitType_e {
        /**
         * 縦横を自動で判断する
         */
        FitType_Auto,

        /**
         * 水平方向をフィットさせる
         */
        FitType_Horizontal,

        /**
         * 縦方向をフィットさせる
         */
        FitType_Vertical,
    };

    VirtualDisplay() {
        realDisplaySize.set(720, 1280);
        virtualDisplaySize.set(480, 800);
        scaling = 1.0f;
        drawingArea.setXYWH(0, 0, 1, 1);
    }

    virtual ~VirtualDisplay() {

    }

    /**
     * 実ディスプレイアスペクト比を取得する
     */
    float getRealDisplayAspect() const {
        return realDisplaySize.x / realDisplaySize.y;
    }

    /**
     * 仮想ディスプレイアスペクト比を取得する
     */
    float getVirtualDisplayAspect() const {
        return virtualDisplaySize.x / virtualDisplaySize.y;
    }

    /**
     * 実ディスプレイサイズを設定する。
     *
     * @param width 幅
     * @param height 高さ
     */
    void setRealDisplaySize(const float width, const float height) {
        realDisplaySize.set(width, height);
    }

    /**
     * 仮想ディスプレイのサイズを指定する。
     * 自動的に全画面が収まるようにフィットさせる。
     * @param width 仮想ディスプレイ幅
     * @param height 仮想ディスプレイ高さ
     */
    void setVirtualDisplaySize(const float width, const float height) {
        virtualDisplaySize.set(width, height);
    }

    /**
     * 現在の設定に合わせて、Viewportを更新する。
     *
     * @param type 画面をフィットさせる方法
     */
    void updateViewport(const FitType_e type) {
        //! 縦横比を計算
        const float mulX = realDisplaySize.x / virtualDisplaySize.x;
        const float mulY = realDisplaySize.y / virtualDisplaySize.y;

        //! スケーリング値が小さい方にあわせて、描画先を設定する。
        if (type == FitType_Auto) {
            scaling = jc::min(mulX, mulY);
        } else if (type == FitType_Vertical) {
            scaling = mulY;
        } else if (type == FitType_Horizontal) {
            scaling = mulX;
        }

        drawingArea.setXYWH(0, 0, virtualDisplaySize.x * scaling, virtualDisplaySize.y * scaling);
        drawingArea.round();

        //! ジャストフィット用の補正を行う。
        if (jc::equals(scaling, mulX) && !jc::equals(drawingArea.width(), realDisplaySize.x)) {
            drawingArea.right = realDisplaySize.x;
        }
        if (jc::equals(scaling, mulY) && !jc::equals(drawingArea.height(), realDisplaySize.y)) {
            drawingArea.bottom = realDisplaySize.y;
        }

        //! 描画位置をセンタリングする
        const s32 offsetX = (s32) (realDisplaySize.x - drawingArea.width());
        const s32 offsetY = (s32) (realDisplaySize.y - drawingArea.height());
        drawingArea.moveTo(offsetX / 2, offsetY / 2);
        drawingArea.round();

        //! 幅が偶数じゃない場合、丸めを行う。
        if (((s32) drawingArea.width()) % 2 != 0) {
            drawingArea.right += 1;
        }
        //! 高さが偶数じゃない場合、丸めを行う。
        if (((s32) drawingArea.height()) % 2 != 0) {
            drawingArea.bottom += 1;
        }
    }

    /**
     * 仮想ディスプレイ座標を上端に合わせる
     */
    void fitVirtualToTop() {
        drawingArea.moveToTop(0);
    }

    void fitVirtualToBottom() {
        drawingArea.moveToBottom(realDisplaySize.y);
    }

    /**
     * 左端に合わせる
     */
    void fitVirtualToLeft() {
        drawingArea.moveToLeft(0);
    }

    /**
     * 右端に合わせる
     */
    void fitVirtualToRight() {
        drawingArea.moveToRight(realDisplaySize.x);
    }

    /**
     * 実際の描画エリアを取得する
     * レンダリングサーフェイスに対し、この値をviewportする。
     */
    const RectF& getDisplayViewport() const {
        return drawingArea;
    }

    /**
     * 仮想ディスプレイサイズを取得する
     */
    const Vector2f& getVirtualDisplaySize() const {
        return virtualDisplaySize;
    }

    /**
     * 描画エリアの幅を取得する
     * @return 描画エリアの幅
     */
    s32 getDisplayViewportWidth() const {
        return (s32) drawingArea.width();
    }

    /**
     * 描画エリアの高さを取得する
     * @return 描画エリアの高さ
     */
    s32 getDisplayViewportHeight() const {
        return (s32) drawingArea.height();
    }

    /**
     * 仮想ディスプレイに対して、実ディスプレイが何倍あるかのスケーリング値を取得する。<BR>
     * 仮想ディスプレイのほうが小さい場合、 > 1.0f<BR>
     * 実ディスプレイのほうが小さい場合、 < 1.0f<BR>
     * となる。
     *
     * @return　デバイスのスケーリング率
     */
    float getDeviceScaling() const {
        return scaling;
    }

    /**
     * 仮想ディスプレイの横サイズの中央を取得する。
     * @return 仮想ディスプレイのX中央値
     */
    s32 getVirtualDisplayCenterX() const {
        return (s32) virtualDisplaySize.x / 2;
    }

    /**
     * 仮想ディスプレイの縦サイズの中央を取得する。
     * @return 仮想ディスプレイのX中央値
     */
    s32 getVirtualDisplayCenterY() const {
        return (s32) virtualDisplaySize.y / 2;
    }

    s32 getRealDisplayWidth() const {
        return (s32) realDisplaySize.x;
    }

    s32 getRealDisplayHeight() const {
        return (s32) realDisplaySize.y;
    }

    /**
     * 仮想ディスプレイの横サイズを取得する。
     * @return 仮想ディスプレイの幅
     */
    s32 getVirtualDisplayWidth() {
        return (s32) virtualDisplaySize.x;
    }

    /**
     * 仮想ディスプレイの縦サイズを取得する
     * @return 仮想ディスプレイの高さ
     */
    s32 getVirtualDisplayHeight() {
        return (s32) virtualDisplaySize.y;
    }

    /**
     * 指定した座標が画面外だったらtrueを返す。
     *
     * @param realPos 実際のデバイス座標
     * @return 画面外だったらtrue
     */
    jcboolean isOutsideReal(const Vector2f &realPos) const {
        return !drawingArea.isIntersect(realPos.x, realPos.y);
    }

    /**
     * 指定した座標が画面外だったらtrueを返す。
     *
     * @param left X座標
     * @param top Y座標
     * @param width 幅
     * @param height 高さ
     * @return 外側だったらtrue
     */
    jcboolean isOutsideReal(const s32 left, const s32 top, const s32 width, const s32 height) const {
        return !drawingArea.isIntersectXYWH(left, top, width, height);
    }

    /**
     * 仮想ディスプレイの外だったらtrueを返す
     * 引数座標は全て仮想ディスプレイサイズで設定する。
     * @param left X座標
     * @param top Y座標
     * @param width 幅
     * @param height 高さ
     * @return
     */
    jcboolean isOutsideVirtual(const s32 left, const s32 top, const s32 width, const s32 height) const {
        const int right = (left + width);
        const int bottom = (top + height);

        //! 右側が0を下回っている、左側が画面サイズよりも大きい、下が画面から見切れている、上が画面から見切れている、いずれかがヒット
        return right < 0 || left > (int) virtualDisplaySize.x || bottom < 0 || top > (int) virtualDisplaySize.y;
    }

    /**
     * 実際のピクセル位置を仮想ディスプレイ位置に変換する。
     *
     * @param realPos Surface上の実座標
     * @return
     */
    Vector2f projectionVirtualDisplayPosition(const Vector2f &realPos) const {
        Vector2f result((realPos.x - drawingArea.left) / scaling, (realPos.y - drawingArea.top) / scaling);
        return result;
    }

    /**
     * 仮想ディスプレイ座標を実ディスプレイ座標に変換する
     */
    Vector2f projectionRealDisplayPosition(const Vector2f &virtualPos) const {
        Vector2f result((virtualPos.x * scaling) + drawingArea.left, (virtualPos.y * scaling) + drawingArea.top);
        return result;
    }

    /**
     * 実際のピクセル位置を正規化座標系に変換する。 GL正規化座標系のため、下側が0.0、上側が1.0となる。
     * この正規化座標は仮想ディスプレイ内の正規化座標に当たる。
     *
     * @param realPos Surface上の実座標
     * @return
     */
    Vector2f projectionNormalizePosition(const Vector2f &realPos) {
        Vector2f result = projectionVirtualDisplayPosition(realPos);
        result.x /= virtualDisplaySize.x;
        result.y /= virtualDisplaySize.y;
        result.y = 1.0f - result.y;
        return result;
    }

    /**
     * 実際のピクセル位置を正規化座標系に変換する。
     * この値は、仮想ディスプレイに対するUVとして動作する。
     * 2D座標であるため、左上が原点(0, 0)であり、右下が(1, 1)となる。
     *
     * @param realPos Surface上の実座標
     * @return
     */
    Vector2f projectionNormalizePosition2D(const Vector2f realPos) {
        Vector2f result = projectionVirtualDisplayPosition(realPos);
        result.x /= virtualDisplaySize.x;
        result.y /= virtualDisplaySize.y;
        return result;
    }
};

typedef jc_sp<VirtualDisplay> MVirtualDisplay;

}

#endif /* VIRTUALDISPLAY_HPP_ */
