/*
 * Mesh.cpp
 *
 *  Created on: 2012/12/17
 */

#include "jcfbx/node/Mesh.h"
#include "jc/math/Math.h"

namespace jc {
namespace fbx {

Mesh::Mesh(KFbxNode *meshNode, s32 nodeNumber) :
        Node(meshNode, nodeNumber) {
    jclogf("Mesh(NodeNumber %d)", nodeNumber);

    if (!meshNode->GetMesh()) {
        throw create_exception_t(FbxException, FbxException_NoMesh);
    }

    // まずは頂点情報をまとめあげる
    {
        KFbxMesh *mesh = meshNode->GetMesh();
        VertexContainer vertices;
        Mesh::createPositions(&vertices.positions, mesh);
        Mesh::createCoords((std::vector<Vector2f>*) &vertices.coords, mesh);

        jclogf("pos(%d) uv(%d)", vertices.positions.size(), vertices.coords.size());
    }
}

Mesh::~Mesh() {

}

MMesh Mesh::createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager) {

    MMesh result(new Mesh(node, importManager->nextNodeId()));

    return result;
}

void Mesh::createPositions(std::vector<Vector3f> *result, KFbxMesh *mesh) {
    const s32 ctrlNum = mesh->GetControlPointsCount();
    KFbxVector4* points = mesh->GetControlPoints();

    // 頂点を末尾へ追加する
    for (s32 i = 0; i < ctrlNum; ++i) {
        result->push_back(Vector3f(points[i][0], points[i][1], points[i][2]));
    }
}

void Mesh::createCoords(std::vector<Vector2f> *result, KFbxMesh *mesh) {
    jclogf("elementUV(%d)", mesh->GetElementUVCount());

    const s32 layerNum = mesh->GetElementUVCount();
    for (s32 i = 0; i < layerNum; ++i) {

        KFbxLayerElementUV *uvElement = mesh->GetElementUV(i);
//! UV
        const s32 UVNum = uvElement->GetDirectArray().GetCount();
        const s32 indexNum = uvElement->GetIndexArray().GetCount();
        const s32 size = UVNum > indexNum ? UVNum : indexNum;

        // マッピングモード・リファレンスモード別にUV取得
        const KFbxLayerElement::EMappingMode mappingMode = uvElement->GetMappingMode();
        const KFbxLayerElement::EReferenceMode refMode = uvElement->GetReferenceMode();

        if (mappingMode == KFbxLayerElement::eByPolygonVertex) {
            if (refMode == KFbxLayerElement::eDirect) {
                // 直接取得
                for (int i = 0; i < size; ++i) {
                    Vector2f v((float) uvElement->GetDirectArray().GetAt(i)[0], (float) uvElement->GetDirectArray().GetAt(i)[1]);
                    result->push_back(v);
                }
                return;
            } else if (refMode == KFbxLayerElement::eIndexToDirect) {
                // インデックスから取得
                for (int i = 0; i < size; ++i) {
                    const s32 index = uvElement->GetIndexArray().GetAt(i);
                    Vector2f v((float) uvElement->GetDirectArray().GetAt(index)[0], (float) uvElement->GetDirectArray().GetAt(index)[1]);
                    result->push_back(v);
                }
                return;
            } else {
                jclogf("not support refmode(%d)", refMode);
                throw create_exception(RuntimeException, "refMode Not Support");
            }
        }
    }
}

}
}
