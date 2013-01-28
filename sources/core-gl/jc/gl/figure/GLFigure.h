/*
 * figure.h
 *
 *  Created on: 2012/12/19
 */

#ifndef __GLFIGURE_H__
#define __GLFIGURE_H__

#include    "jc/graphics/figure/Figure.h"

#include    "jc/math/Transform.h"
#include    "jc/graphics/figure/data/FigureDataLoader.h"
#include    "jc/graphics/figure/TextureTable.h"
#include    "jc/graphics/figure/animator/AnimationClip.h"
#include    "jc/gl/TextureImage.h"

#include    "jc/gl/IndexBufferObject.h"
#include    "jc/gl/VertexBufferObject.h"

#include    <vector>

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

    /**
     * ボーンのインデックス
     */
    float bone_indices[SIMPLE_BONE_NUM];

    /**
     * ボーンの重み情報
     */
    float bone_weights[SIMPLE_BONE_NUM];
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

    struct {
        /**
         * レンダリング時のUオフセット
         */
        float u;

        /**
         * レンダリング時のVオフセット
         */
        float v;
    } offset;

    GLFigureMaterial() {
        use_texture = jcfalse;
        offset.u = offset.v = 0;
    }
};

typedef jc_sp<GLFigureMaterial> MGLFigureMaterial;

/**
 * VBO
 */
typedef VertexBufferObject<GLFigureVertex> FigureVertexBufferObject;

/**
 * FbxFigureを構築するメッシュ情報
 */
typedef _MeshFragment<GLFigureVertex, GLFigureMaterial, jc_sp<FigureVertexBufferObject>, MIndexBufferObject, u16> GLFigureMeshFragment;

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
     * ノード名
     */
    String name;

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
     * デフォルトテイクの逆行列情報
     */
    Matrix4x4 matrix_default_invert;

    /**
     * 現在の行列
     */
    Matrix4x4 matrix_current_world;
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
    enum {
        /**
         * 一度の描画で利用できるボーンの最大数
         */
        MAX_BONE_TABLE = 28
    };
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
             * 頂点ウェイトインデックス
             */
            s32 weight_indeices;

            /**
             * 頂点ウェイト
             */
            s32 weight;

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

            /**
             * ボーンの頂点テーブル
             */
            s32 bones;

            /**
             * UVオフセット値
             */
            s32 uv_offset;
        } uniforms;

        ShaderParams() {
            attributes.position = attributes.uv = attributes.normals = attributes.weight = attributes.weight_indeices = ATTRIBUTE_DISABLE_INDEX;
            uniforms.tex_0 = uniforms.bones = uniforms.uv_offset = UNIFORM_DISABLE_INDEX;
        }
    };
private:
    /**
     * 全ノードは直線的に管理してしまう。
     */
    std::vector<MFigureNode> nodes;

    /**
     * マテリアルキャッシュ
     */
    std::vector<MGLFigureMaterial> materials;

    /**
     * テクスチャテーブル
     */
    MGLTextureTable textures;

    /**
     * GL用デバイス
     */
    MDevice device;

    /**
     * 転送用のボーンテーブル
     * pick tableを介してノードごとのMatrixを並べる
     * node[ pick_table[index] ]のMatrixが格納される
     */
    Matrix4x4 bone_table[MAX_BONE_TABLE];

    /**
     * 指定したノード番号のボーンテーブルを用意する
     */
    void _enumBones(GLFigureMeshFragment *pFragment, GLFigureMeshFragment::DrawingContext *pContext);

    /**
     * 指定した番号のノードを描画・探索する
     */
    void _rendering(const u32 nodeNumber, const GLFigure::ShaderParams *params);

    /**
     * 逆行列を作成する
     */
    void _initializeInvertMatrices(const u32 nodeNumber, const Matrix4x4 &parent);

    /**
     * アニメーションを当てる
     */
    void _posing(AnimationClip *animation, const u32 nodeNumber, const Matrix4x4 &parent);

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

    /**
     * アニメーション情報に合わせて、ポーズを取らせる。
     * 行列はAnimationClip側で計算するため、単純な行列置き換えのみを行う。
     */
    virtual void posing(AnimationClip *animation);

    /**
     * レンダリング時のUV値オフセットを設定する
     */
    virtual void setUVOffset(const String &mat_name, const float u, const float v);

    /**
     * 各ノードの逆行列を作成する.
     * 初回に一度だけ呼び出せばいい。
     *
     * 各頂点はglobal行列適用済みのため、一度逆行列を通してローカルに落とし込んだ後、再度現在のボーンに合わせて行列を当てる必要がある。
     */
    virtual void initializeInvertMatrices();

    /**
     * マテリアルを登録する。
     * 同一名・同一テクスチャのマテリアルがあれば、そのマテリアルを返してshareできるようにする。
     * 無い場合はキャッシュに新規登録し、oldをそのまま返す。
     */
    virtual MGLFigureMaterial putMaterial(const MGLFigureMaterial old);
};

/**
 * managed
 */
typedef jc_sp<GLFigure> MGLFigure;

}
}

#endif // __GLFIGURE_H__
