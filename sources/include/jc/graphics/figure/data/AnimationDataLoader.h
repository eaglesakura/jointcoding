/*
 * AnimationDataLoader.h
 *
 *  Created on: 2013/01/08
 */

#ifndef ANIMATIONDATALOADER_H_
#define ANIMATIONDATALOADER_H_

#include    "jointcoding.h"
#include    "jc/graphics/figure/animator/AnimationClip.h"
#include    "jc/graphics/figure/data/FigureDataFactory.h"

namespace jc {

class AnimationDataLoader: public Object {
    MFigureDataFactory factory;

public:
    AnimationDataLoader(MFigureDataFactory factory) {
        this->factory = factory;
    }

    virtual ~AnimationDataLoader() {

    }

    /**
     * アニメーションの読み込みを行う
     */
    virtual MAnimationClip load(const u32 clipNumber);
};

}

#endif /* ANIMATIONDATALOADER_H_ */
