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
#include "jcfbx/attribute/Material.h"

namespace jc {
namespace fbx {

class Mesh;
typedef jc_sp<Mesh> MMesh;
/**
 * FBXから読み込んだメッシュ情報を管理する
 */
class Mesh: public jc::fbx::Node {
protected:
    // vertices
    static void createCoords(std::vector<Vector2f> *result, KFbxMesh *mesh);
    static void createPositions(std::vector<Vector3f> *result, KFbxMesh *mesh);
    static void createNormals(std::vector<Vector3f> *result, KFbxMesh *mesh);

    // polygons
    static void createMaterials(std::vector<Material> *result, KFbxMesh *mesh);

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
