/*
 * Mesh.cpp
 *
 *  Created on: 2012/12/17
 */

#include "jcfbx/node/Mesh.h"

namespace jc {
namespace fbx {

Mesh::Mesh(KFbxNode *meshNode, s32 nodeNumber) :
        Node(meshNode, nodeNumber) {
    jclogf("Mesh(NodeNumber %d)", nodeNumber);

    if (!meshNode->GetMesh()) {
        throw create_exception_t(FbxException, FbxException_NoMesh);
    }

    createPositions(&vertices.positions, meshNode->GetMesh());
}

Mesh::~Mesh() {

}

MMesh Mesh::createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager) {

    MMesh result(new Mesh(node, importManager->nextNodeId()));

    return result;
}

s32 Mesh::createPositions(std::vector<Vector3f> *result, KFbxMesh *mesh) {
    const u32 beginSize = result->size();

    const s32 ctrlNum = mesh->GetControlPointsCount();
    KFbxVector4* points = mesh->GetControlPoints();

    // 頂点を末尾へ追加する
    for (s32 i = 0; i < ctrlNum; ++i) {
        result->push_back(Vector3f(points[i][0], points[i][1], points[i][2]));
    }

    return result->size() - beginSize;
}

}
}
