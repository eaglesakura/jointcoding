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
        float offset;
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
     * アニメーションの時刻オフセットを設定する
     */
    virtual void setAnimationOffset(const float offset) {
        this->animation.offset = offset;
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
    virtual jcboolean nextAnimation() {
        if (animation.clip) {
            const float next_frame = animation.clip->getCurrentFrame() + animation.offset;
            animation.clip->setCurrentFrame(next_frame);

            // anim finish?
            return next_frame >= animation.clip->getAnimationLength();
        }

        return jctrue;
    }

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
    virtual void rendering(FigureRenderer *render) const;
};

/**
 * managed
 */
typedef jc_sp<Showcase> MShowcase;

}
}

#endif /* SHOWCASE_H_ */
