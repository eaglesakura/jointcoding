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

#include    "jc/gl/IndexBufferObject.h"
#include    "jc/gl/VertexBufferObject.h"

namespace jc {
namespace gl {

struct GLFigureVertex {
    /**
     * 位置情報
     */
    Vector3f position;

    /**
     * 頂点のUV
     */
    Vector2f uv;
};

typedef VertexBufferObject<GLFigureVertex> FigureVertexBufferObject;

/**
 * FbxFigureを構築するメッシュ情報
 */
typedef _MeshFragment<GLFigureVertex, jc::FigureMaterial, jc_sp<FigureVertexBufferObject>, MIndexBufferObject> GLFigureMeshFragment;

/**
 * managed
 */
typedef jc_sp<GLFigureMeshFragment> MGLFigureMeshFragment;

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
    std::vector<MGLFigureMeshFragment> renderingFragments;

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
     * ノードのレンダリングを行う
     */
    virtual void onNodeRendering(FigureNode *node);
public:
    GLFigure();

    virtual ~GLFigure();

    /**
     * 指定した番号のノードを取得する
     */
    inline FigureNode* getNodePtr(const s32 nodeNumber) const {
        return nodes[nodeNumber].get();
    }

    /**
     * レンダリングを開始する
     */
    virtual void rendering();

    /**
     * フィギュアの情報を読み込む。
     * 不要な情報は切り捨ててもいい。
     */
    virtual void load(FigureDataLoader *loader);

    /**
     * セットアップしたノードをそのままコピーする
     */
    virtual void setNodes(const std::vector<MFigureNode> &origin) {
        nodes = origin;
    }

    /**
     * ノード数をリサイズする
     */
    virtual void resizeNodeCount(const u32 num) {
        nodes.resize(num);
        for (u32 i = 0; i < num; ++i) {
            MFigureNode node = nodes[i];
            if (!node) {
                nodes[i].reset(new FigureNode());
            }
        }
    }
};

/**
 * managed
 */
typedef jc_sp<GLFigure> MGLFigure;}
}

