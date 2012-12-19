/*
 * Mesh.h
 *
 *  Created on: 2012/12/17
 */

#ifndef JCFBXMESH_H_
#define JCFBXMESH_H_

#include    "jcfbx/node/Node.h"
#include    "jc/graphics/figure/mesh/Figure.h"

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

    static MMesh createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager);
};

}
}

#endif /* MESH_H_ */
