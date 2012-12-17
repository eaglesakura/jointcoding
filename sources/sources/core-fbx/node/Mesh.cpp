/*
 * Mesh.cpp
 *
 *  Created on: 2012/12/17
 */

#include "jcfbx/node/Mesh.h"
#include "jc/math/Math.h"
#include "jcfbx/attribute/VertexContainer.h"
#include "jcfbx/attribute/IndicesContainer.h"

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

        // 頂点情報
        VertexContainer vertices;
        // ポリゴン情報
        IndicesContainer indices;

        // 頂点情報を整理する
        {
            Mesh::createPositions(&vertices.positions, mesh);
            Mesh::createCoords((std::vector<Vector2f>*) &vertices.coords, mesh);
            Mesh::createNormals(&vertices.normals, mesh);
        }
        // ポリゴン情報を整理する
        {
            Mesh::createMaterials(&indices.materials, mesh);
        }

        jclogf("pos(%d) uv(%d) normal(%d) mat(%d)", vertices.positions.size(), vertices.coords.size(), vertices.normals.size(), indices.materials.size());
    }
}

Mesh::~Mesh() {

}

MMesh Mesh::createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager) {

    MMesh result(new Mesh(node, importManager->nextNodeId()));

    return result;
}

void Mesh::createNormals(std::vector<Vector3f> *result, KFbxMesh *mesh) {
    const s32 layerNum = mesh->GetElementNormalCount();

    for (s32 i = 0; i < layerNum; ++i) {
        const KFbxLayerElementNormal *normals = mesh->GetElementNormal(i);
        //! normal
        if (normals && normals->GetReferenceMode() == KFbxLayerElement::eDirect) {
            const s32 normalNum = normals->GetDirectArray().GetCount();
            for (s32 k = 0; k < normalNum; ++k) {
                FbxVector4 v = normals->GetDirectArray().GetAt(k);
                result->push_back(Vector3f(v[0], v[1], v[2]));
            }
            return;
        }
    }
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

void Mesh::createMaterials(std::vector<Material> *result, KFbxMesh *mesh) {
    const s32 materialNum = mesh->GetNode()->GetMaterialCount();

    if (materialNum == 0) {
        result->push_back(Material());
    }

    for (s32 i = 0; i < materialNum; ++i) {
        KFbxSurfaceMaterial *material = mesh->GetNode()->GetMaterial(i);
        Material m;

        m.name = material->GetName();
        if (material->GetClassId().Is(KFbxSurfaceLambert::ClassId)) {
            KFbxSurfaceLambert* mat = (KFbxSurfaceLambert*) material;
            m.ambient.RGBAf(mat->Ambient.Get()[0], mat->Ambient.Get()[1], mat->Ambient.Get()[2], (float) mat->AmbientFactor);
            m.diffuse.RGBAf(mat->Diffuse.Get()[0], mat->Diffuse.Get()[1], mat->Diffuse.Get()[2], (float) mat->DiffuseFactor);
            m.emissive.RGBAf(mat->Emissive.Get()[0], mat->Emissive.Get()[1], mat->Emissive.Get()[2], (float) mat->EmissiveFactor);
        } else if (material->GetClassId().Is(KFbxSurfacePhong::ClassId)) {
            KFbxSurfacePhong* mat = (KFbxSurfacePhong*) material;

            m.ambient.RGBAf(mat->Ambient.Get()[0], mat->Ambient.Get()[1], mat->Ambient.Get()[2], (float) mat->AmbientFactor);
            m.diffuse.RGBAf(mat->Diffuse.Get()[0], mat->Diffuse.Get()[1], mat->Diffuse.Get()[2], (float) mat->DiffuseFactor);
            m.emissive.RGBAf(mat->Emissive.Get()[0], mat->Emissive.Get()[1], mat->Emissive.Get()[2], (float) mat->EmissiveFactor);
        }
        KFbxProperty prop = material->FindProperty(KFbxSurfaceMaterial::sDiffuse);
        const s32 texNum = prop.GetSrcObjectCount<KFbxTexture>();
        if (texNum) {
            KFbxTexture *texture = prop.GetSrcObject<KFbxTexture>(0);
            if (texture && strlen(texture->GetName())) {
                m.textureName = texture->GetName();


                jclogf("texture(%s)", m.textureName.c_str());
            }
        }

        result->push_back(m);
    }
}

}
}
