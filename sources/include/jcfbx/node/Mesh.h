/*
 * Mesh.h
 *
 *  Created on: 2012/12/17
 */

#ifndef JCFBXMESH_H_
#define JCFBXMESH_H_

#include "jcfbx/node/Node.h"
#include <vector>
#include "jc/math/Vec2.h"
#include "jc/math/Vec3.h"
#include "jcfbx/attribute/VertexContainer.h"
#include "jcfbx/attribute/IndicesContainer.h"

namespace jc {
namespace fbx {

class Mesh;
typedef jc_sp<Mesh> MMesh;
/**
 * FBXから読み込んだメッシュ情報を管理する
 */
class Mesh: public jc::fbx::Node {
protected:

    Mesh(KFbxNode *meshNode, s32 nodeNumber);
public:
    virtual ~Mesh();

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
