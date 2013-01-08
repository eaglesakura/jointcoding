/*
 * AnimationDataLoader.cpp
 *
 *  Created on: 2013/01/08
 */

#include    "jc/graphics/figure/data/AnimationDataLoader.h"

namespace jc {

/**
 * アニメーションの読み込みを行う
 */
MAnimationClip AnimationDataLoader::load(const u32 clipNumber) {
    MAnimationClip result;

    u32 node_num = 0;

    // ノード数を求める
    {
        MBinaryInputStream stream = factory->openFigureInfo();
        node_num = stream->readU16();
    }

    // ノード数だけ、アニメーションを読み込む
    MAnimationGroup group(new AnimationGroup(node_num));

    for (u32 i = 0; i < node_num; ++i) {

        // translate
        {
            MBinaryInputStream stream = factory->openAnimationData(AnimationDataType_Translate, clipNumber, i);
            const String name = stream->readString();
            const u32 key_num = stream->readU16();

            for (u32 k = 0; k < key_num; ++k) {
                TranslateKey keyFrame;
                keyFrame.frame = stream->readU16();
                keyFrame.value.x = stream->readFixed32();
                keyFrame.value.y = stream->readFixed32();
                keyFrame.value.z = stream->readFixed32();

                // 現在のノードにキーフレームを書き込む
                group->animations[i].addTranslateAnimation(keyFrame);
            }
        }

        // rotate
        {
            MBinaryInputStream stream = factory->openAnimationData(AnimationDataType_Rotate, clipNumber, i);
            const String name = stream->readString();
            const u32 key_num = stream->readU16();

            for (u32 k = 0; k < key_num; ++k) {
                RotateKey keyFrame;
                keyFrame.frame = stream->readU16();
                keyFrame.value.x = stream->readFixed32();
                keyFrame.value.y = stream->readFixed32();
                keyFrame.value.z = stream->readFixed32();
                keyFrame.value.w = stream->readFixed32();

                // 現在のノードにキーフレームを書き込む
                group->animations[i].addRotateAnimation(keyFrame);
            }
        }

        // scale
        {
            MBinaryInputStream stream = factory->openAnimationData(AnimationDataType_Scale, clipNumber, i);
            const String name = stream->readString();
            const u32 key_num = stream->readU16();

            for (u32 k = 0; k < key_num; ++k) {
                ScaleKey keyFrame;
                keyFrame.frame = stream->readU16();
                keyFrame.value.x = stream->readFixed32();
                keyFrame.value.y = stream->readFixed32();
                keyFrame.value.z = stream->readFixed32();

                // 現在のノードにキーフレームを書き込む
                group->animations[i].addScaleAnimation(keyFrame);
            }
        }
    }

    // 全て読み込み終わったら生成する
    result.reset(new AnimationClip(group));

    return result;
}

}
