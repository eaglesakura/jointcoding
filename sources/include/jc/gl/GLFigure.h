/*
 * figure.h
 *
 *  Created on: 2012/12/19
 */

#include    "jc/graphics/figure/Figure.h"

#include    "jc/math/Transform.h"
#include    "jc/graphics/figure/data/FigureDataLoader.h"
#include    "jc/graphics/figure/TextureTable.h"
#include    "jc/gl/TextureImage.h"

#include    "jc/gl/IndexBufferObject.h"
#include    "jc/gl/VertexBufferObject.h"

namespace jc {
namespace gl {

/**
 * GLフィギュア用の頂点情報
 */
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

/**
 * Figure用のマテリアル情報
 */
class GLFigureMaterial: public FigureMaterial {
public:
    /**
     * キャッシュしたテクスチャ.
     * NULLの場合、テクスチャテーブルから持ちだしてくる。
     */
    MTextureImage texture;

    /**
     * テクスチャを使用する場合はjctrue
     */
    jcboolean use_texture;

    GLFigureMaterial() {
        use_texture = jcfalse;
    }
};

/**
 * VBO
 */
typedef VertexBufferObject<GLFigureVertex> FigureVertexBufferObject;

/**
 * FbxFigureを構築するメッシュ情報
 */
typedef _MeshFragment<GLFigureVertex, GLFigureMaterial, jc_sp<FigureVertexBufferObject>, MIndexBufferObject> GLFigureMeshFragment;

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
    Transform defTransform;

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
 * GLで利用するテクスチャテーブル
 */
typedef TextureTable<TextureImage> GLTextureTable;

/**
 * managed
 */
typedef jc_sp<GLTextureTable> MGLTextureTable;

/**
 * 3D上に表示するフィギュアを構築する
 */
class GLFigure: public Object {
public:
    /**
     * 頂点のインデックス情報を渡す
     * 利用しない場合はデフォルトのまま
     */
    struct ShaderParams {
        struct {
            /**
             * 位置情報
             */
            s32 position;

            /**
             * UV
             */
            s32 uv;

            /**
             * 法線
             */
            s32 normals;

        } attributes;

        struct {
            /**
             * メインテクスチャのuniform値
             */
            s32 tex_0;
        } uniforms;

        ShaderParams() {
            attributes.position = attributes.uv = attributes.normals = ATTRIBUTE_DISABLE_INDEX;
            uniforms.tex_0 = UNIFORM_DISABLE_INDEX;
        }
    };
private:
    /**
     * 全ノードは直線的に管理してしまう。
     */
    std::vector<MFigureNode> nodes;

    /**
     * テクスチャテーブル
     */
    MGLTextureTable textures;

    /**
     * GL用デバイス
     */
    MDevice device;

    /**
     * 指定した番号のノードを描画・探索する
     */
    void _rendering(const u32 nodeNumber, const GLFigure::ShaderParams *params);

protected:

    /**
     * ノードのレンダリングを行う
     */
    virtual void onNodeRendering(const s32 nodeNumber, FigureNode *node, const GLFigure::ShaderParams *params);
public:
    GLFigure();

    virtual ~GLFigure();

    /**
     * GL用デバイスを設定する
     */
    virtual void setDevice(const MDevice device) {
        this->device = device;
    }

    /**
     * GL用デバイスを取得する
     */
    virtual MDevice getDevice() const {
        return device;
    }

    /**
     * テクスチャ設定
     */
    virtual void setTextureTable(MGLTextureTable table) {
        textures = table;
    }

    /**
     * 指定した番号のノードを取得する
     */
    inline FigureNode* getNodePtr(const s32 nodeNumber) const {
        return nodes[nodeNumber].get();
    }

    /**
     * レンダリングを開始する
     */
    virtual void rendering(const GLFigure::ShaderParams *params);

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

