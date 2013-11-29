/*
 * ShowCase.h
 *
 *  Created on: 2013/01/18
 */

#ifndef SHOWCASE_H_
#define SHOWCASE_H_

#include    "jointcoding.h"
#include    "jc/math/Transform.h"
#include    "jc/gl/figure/FigureRenderer.h"
#include    "jc/graphics/figure/animator/AnimationClip.h"
#include    "jc/scene/LoopController.h"

namespace jc {
namespace gl {

/**
 * 3Dフィギュアのレンダリングに必要な情報をひとまとめにしたクラス。
 * 3Dフィギュアは複数のshowcaseに共有される。
 */
class Showcase: public Object {
    /**
     * 描画用フィギュア
     */
    MGLFigure figure;

    struct {
        /**
         * 再生対象のアニメーションクリップ
         */
        MAnimationClip clip;

        /**
         * アニメーションのオフセット値。
         * 通常1.0
         */
        float speed;

        /**
         * アニメーション作成時のフレームレート
         */
        u32 originRate;
    } animation;

    /**
     * 位置座標を決める
     */
    Transform transform;
public:
    Showcase();

    virtual ~Showcase();

    /**
     *
     */
    virtual void setFigure(const MGLFigure figure) {
        this->figure = figure;
    }

    virtual MGLFigure getFigure() const {
        return figure;
    }

    /**
     * アニメーションを設定する
     */
    virtual void setAnimation(const MAnimationClip animation) {
        this->animation.clip = animation;
    }

    /**
     * アニメーションフレームを設定する
     */
    virtual void setAnimationFrame(const float frame) {
        this->animation.clip->setCurrentFrame(frame);
    }

    /**
     * 元モデル作成時のフレームレートを設定する。
     */
    virtual void setAnimationFrameRate(const u32 rate) {
        this->animation.originRate = rate;
    }

    /**
     * アニメーションの時刻オフセットを設定する
     */
    virtual void setAnimationSpeed(const float speed) {
        this->animation.speed = speed;
    }

    /**
     * アニメーションを取得する
     */
    virtual MAnimationClip getAnimation() const {
        return this->animation.clip;
    }

    /**
     * アニメーションを進める。
     */
    virtual jcboolean nextAnimation(const MLoopController controller);

    /**
     *
     */
    virtual Transform* getTransform() {
        return &transform;
    }

    /**
     *
     */
    virtual const Transform* getTransform() const {
        return &transform;
    }

    /**
     * 描画を行う
     */
    virtual void rendering(MGLState state, FigureRenderer *render) const;
};

/**
 * managed
 */
typedef jc_sp<Showcase> MShowcase;

}
}

#endif /* SHOWCASE_H_ */
