/*
 * Mesh.h
 *
 *  Created on: 2012/12/17
 */

#ifndef JCFBXMESH_H_
#define JCFBXMESH_H_

#include    "jcfbx/node/Node.h"
#include    "jc/graphics/figure/Figure.h"

namespace jc {
namespace fbx {

class Mesh;
typedef jc_sp<Mesh> MMesh;

/**
 * FBXから読み込んだメッシュ情報を管理する
 */
class Mesh: public jc::fbx::Node {
protected:
    /**
     * ビルドを行ったフラグメント情報
     */
    std::vector<MFigureMeshFragment> fragments;

    /**
     * ボーンとして接続されたノード番号
     *
     * boneNodeNumbers[ boneIndex ]がボーン行列として作用する
     */
    std::vector<s32> boneNodeNumbers;

    /**
     * スキニングが有効な場合、trueが設定される
     */
    jcboolean skinning;

    Mesh(KFbxNode *meshNode, s32 nodeNumber);
public:
    virtual ~Mesh();

    /**
     * メッシュの断片を取得する
     */
    virtual MFigureMeshFragment getFragment(const int index) const {
        return fragments[index];
    }

    /**
     * 構築しているフラグメント数を取得する
     */
    virtual s32 getFragmentCount() const {
        return fragments.size();
    }

    /**
     * 基底クラスはMeshノード
     */
    virtual NodeType_e getNodeType() const {
        return NodeType_Mesh;
    }

    /**
     * 再帰的に管理しているマテリアルを列挙する
     */
    virtual void getAllMaterials(std::map<String, MFigureMaterial> *result);

    /**
     * 再帰的に管理している頂点数を数える
     */
    virtual u32 getAllVertexNum() const;

    /**
     * 再帰的に管理しているインデックス数を数える
     */
    virtual u32 getAllIndicesNum() const;

    /**
     * ボーンのリンクを再構築する
     */
    virtual void registerBones();

    /**
     * 出力を行う
     */
    virtual void serialize(FbxExportManager *exportManager);

    static MMesh createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager);
};

}
}

#endif /* MESH_H_ */
