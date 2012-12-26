/*
 * figure.h
 *
 *  Created on: 2012/12/19
 */

#include    "jc/graphics/figure/Figure.h"

#include    "jc/math/EulerTransform.h"
#include    "jc/graphics/figure/data/FigureDataLoader.h"
#include    "jc/graphics/figure/TextureTable.h"
#include    "jc/gl/TextureImage.h"

namespace jc {
namespace gl {

/**
 * フィギュア構築用のノード情報
 */
class FigureNode {
public:
    /**
     * 子ノードのID
     */
    std::vector<s32> children;

    /**
     * ノードのデフォルト行列
     */
    EulerTransform defTransform;

    /**
     * ノード番号
     * 通常、これは通し番号になる
     */
    s32 nodeNumber;

    /**
     * レンダリング用フラグメント情報
     * １フラグメント=１マテリアル
     */
    std::vector<MFigureMeshFragment> renderingFragments;

    /**
     * レンダリング用マテリアル
     */
    std::vector<MFigureMaterial> materials;
};

typedef jc_sp<FigureNode> MFigureNode;

/**
 * 3D上に表示するフィギュアを構築する
 */
class GLFigure: public Object {
    /**
     * 全ノードは直線的に管理してしまう。
     */
    std::vector<MFigureNode> nodes;

    /**
     * テクスチャテーブル
     */
    TextureTable<TextureImage> textures;

protected:
    /**
     * 指定した番号のノードを取得する
     */
    inline FigureNode* getNode(const s32 nodeNumber) const {
        return nodes[nodeNumber].get();
    }

    /**
     * ノードのレンダリングを行う
     */
    virtual void onNodeRendering(FigureNode *node);
public:
    GLFigure();

    virtual ~GLFigure();

    /**
     * レンダリングを開始する
     */
    virtual void rendering();

    /**
     * フィギュアの情報を読み込む。
     * 不要な情報は切り捨ててもいい。
     */
    virtual void load(FigureDataLoader *loader);
};

}
}

