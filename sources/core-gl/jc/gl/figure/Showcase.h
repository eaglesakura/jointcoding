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
     *
     */
    virtual void setAnimation(const MAnimationClip animation) {
        this->animation.clip = animation;
    }

    /**
     * アニメーションを進める。
     */
    virtual void nextAnimation() {
        if (animation.clip) {
            animation.clip->setCurrentFrame(animation.clip->getCurrentFrame() + animation.offset);
        }
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

}
}

#endif /* SHOWCASE_H_ */
