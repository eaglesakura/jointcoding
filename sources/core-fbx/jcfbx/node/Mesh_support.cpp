/*
 * Mesh_support.cpp
 *
 *  Created on: 2012/12/25
 */

#include "jointcoding-fbx.h"
#include <vector>
#include "jc/graphics/figure/mesh/BoneWeight.h"
#include "jcfbx/node/Mesh.h"
#include "jc/math/Math.h"
#include "jcfbx/attribute/VertexContainer.h"
#include "jcfbx/attribute/IndicesContainer.h"
#include "prv_FbxDeformer.h"
#include    "jcfbx/convert/MeshFragmentConverter.h"

#include    "jc/system/StringUtil.h"

namespace jc {
namespace fbx {

void createNormals(std::vector<Vector3f> *result, KFbxMesh *mesh) {
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

/**
 * 位置情報を列挙する
 */
void createPositions(std::vector<Vector3f> *result, KFbxMesh *mesh) {
    const s32 ctrlNum = mesh->GetControlPointsCount();
    KFbxVector4* points = mesh->GetControlPoints();

// 頂点を末尾へ追加する
    for (s32 i = 0; i < ctrlNum; ++i) {
        result->push_back(Vector3f(points[i][0], points[i][1], points[i][2]));
    }
}

/**
 * UV情報を列挙する
 */
void createCoords(std::vector<Vector2f> *result, KFbxMesh *mesh) {
    jclogf("  elementUV(%d)", mesh->GetElementUVCount());

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

void createWeights(std::vector<SimpleBoneWeight> *result, const std::vector<Vector3f> &positions, KFbxMesh *mesh) {

    const s32 deformerCount = mesh->GetDeformerCount(KFbxDeformer::eSkin);

    jclogf("  has deformers(%d)", deformerCount);
    if (!deformerCount) {
        return;
    }

    result->clear();

// 位置数だけ必要になる
    result->resize(positions.size());

    for (int i = 0; i < deformerCount; ++i) {
        KFbxSkin *skin = (KFbxSkin*) mesh->GetDeformer(i, KFbxDeformer::eSkin);

        const s32 clusterCount = skin->GetClusterCount();
        jclogf("  clusters(%d)", clusterCount);

        //! クラスタ（ボーン）を取得する
        for (int boneIndex = 0; boneIndex < clusterCount; ++boneIndex) {
            KFbxCluster *cluster = skin->GetCluster(boneIndex);
            const char* culsterName = cluster->GetName();

            jclogf("    name(%s)", culsterName);

            ConvertedDeformer deformer(cluster);
            const int indices_size = deformer.indices.size();

            // ウェイト情報を登録する
            for (int vertexIndex = 0; vertexIndex < indices_size; ++vertexIndex) {
                (*result)[deformer.indices[vertexIndex]].registerWegight(boneIndex, deformer.weights[vertexIndex]);
            }
        }
    }

// 頂点ウェイトを正規化する
    const int weight_size = result->size();
    for (int i = 0; i < weight_size; ++i) {
        (*result)[i].normalize();

//        jclogf("    vertex[%d] weight(%d = %.3f, %d = %.3f, %d = %.3f, %d = %.3f)", i, (*result)[i].indices[0], (*result)[i].weights[0], (*result)[i].indices[1], (*result)[i].weights[1], (*result)[i].indices[2], (*result)[i].weights[2], (*result)[i].indices[3], (*result)[i].weights[3]);
    }
}

/**
 * マテリアルを列挙する
 */
void createMaterials(std::vector<MFigureMaterial> *result, KFbxMesh *mesh) {
    const s32 materialNum = mesh->GetNode()->GetMaterialCount();

    if (materialNum == 0) {
        MFigureMaterial temp(new FigureMaterial());
        temp->name = "NO_MATERIAL";
        result->push_back(temp);
        return;
    }

// マテリアルを全て集積する
    for (s32 i = 0; i < materialNum; ++i) {
        KFbxSurfaceMaterial *material = mesh->GetNode()->GetMaterial(i);
        MFigureMaterial m(new FigureMaterial());

        m->name = material->GetName();

        if (material->GetClassId().Is(KFbxSurfaceLambert::ClassId)) {
            KFbxSurfaceLambert* mat = (KFbxSurfaceLambert*) material;
            m->ambient.RGBAf(mat->Ambient.Get()[0], mat->Ambient.Get()[1], mat->Ambient.Get()[2], (float) mat->AmbientFactor);
            m->diffuse.RGBAf(mat->Diffuse.Get()[0], mat->Diffuse.Get()[1], mat->Diffuse.Get()[2], (float) mat->DiffuseFactor);
            m->emissive.RGBAf(mat->Emissive.Get()[0], mat->Emissive.Get()[1], mat->Emissive.Get()[2], (float) mat->EmissiveFactor);
        } else if (material->GetClassId().Is(KFbxSurfacePhong::ClassId)) {
            KFbxSurfacePhong* mat = (KFbxSurfacePhong*) material;

            m->ambient.RGBAf(mat->Ambient.Get()[0], mat->Ambient.Get()[1], mat->Ambient.Get()[2], (float) mat->AmbientFactor);
            m->diffuse.RGBAf(mat->Diffuse.Get()[0], mat->Diffuse.Get()[1], mat->Diffuse.Get()[2], (float) mat->DiffuseFactor);
            m->emissive.RGBAf(mat->Emissive.Get()[0], mat->Emissive.Get()[1], mat->Emissive.Get()[2], (float) mat->EmissiveFactor);
        }
        KFbxProperty prop = material->FindProperty(KFbxSurfaceMaterial::sDiffuse);
        const s32 texNum = prop.GetSrcObjectCount<KFbxFileTexture>();
        if (texNum) {
            KFbxFileTexture *texture = prop.GetSrcObject<KFbxFileTexture>(0);
            if (texture && texture->GetFileName()) {

                String fileName = getFileName(texture->GetFileName());
                s32 dot_index = fileName.indexOf(".");
                if (dot_index > 0) {
                    fileName = fileName.substring(0, dot_index);
                }
                m->textureName = fileName;
                /*
                 if (__getFileName(texture->GetFileName())) {
                 m->textureName = __getFileName(texture->GetFileName());
                 }else {
                 }
                 * */
            }
        }

        jclogf("  mat(%s) tex name(%s)", m->name.c_str(), m->textureName.c_str());
        result->push_back(m);
    }
}

//! メッシュ内の三角形数を取得する
static s32 getTriangleCount(KFbxMesh* mesh) {
    s32 ret = 0;
    s32 polyNum = mesh->GetPolygonCount();

    for (s32 i = 0; i < polyNum; ++i) {
        s32 tris = mesh->GetPolygonSize(i) - 2;
        ret += tris;
    }
    return ret;
}

static std::vector<s32> createMaterialIndexArray(KFbxMesh* mesh, s32 layerNum) {
    std::vector<s32> result;
    KFbxLayerElementMaterial *mat = mesh->GetLayer(layerNum)->GetMaterials();
    if (!mat) {
        return result;
    }

    if (mat->GetReferenceMode() != KFbxLayerElementMaterial::eIndex && mat->GetReferenceMode() != KFbxLayerElementMaterial::eIndexToDirect) {
        return result;
    }

    s32 indexArrayCount = mat->GetIndexArray().GetCount();

    for (s32 i = 0; i < indexArrayCount; ++i) {
        s32 num = mat->GetIndexArray().GetAt(i);
        result.push_back(num);
    }

    return result;
}

void createPolygonList(std::vector<ConvertedPolygon> *result, KFbxMesh *mesh) {

// 必要なポリゴン数
    s32 triangleNum = getTriangleCount(mesh);
// quadを含んだFBXポリゴン数
    s32 polyNum = mesh->GetPolygonCount();
//! マテリアル配列を作成する。
    std::vector<s32> materialNumbers = createMaterialIndexArray(mesh, 0);

    s32 current = 0;
    s32 uvIndex = 0;

    result->clear();
    result->resize(triangleNum);

//! 三角形の位置情報はインデックスで、UV情報はポリゴンごとに順に格納されている。
    for (s32 i = 0; i < polyNum; ++i) {
        s32 size = mesh->GetPolygonSize(i);
        jc_sa< s32 > index(new s32[size]);

        for (s32 k = 0; k < size; ++k) {
            index[k] = mesh->GetPolygonVertex(i, k);
        }

        // 四角形ポリは三角形ポリに分解して登録する
        for (s32 k = 0; k < (size - 2); ++k) {
            if (current >= (s32) result->size()) {
                throw create_exception_t(FbxException, FbxException_ArrayIndexBoundsException);
            }

            //! 頂点インデックス
            (*result)[current].position[0] = index[0];
            (*result)[current].position[1] = index[k + 2];
            (*result)[current].position[2] = index[k + 1];
            //! uv
            (*result)[current].attributes[0] = (uvIndex + 0);
            (*result)[current].attributes[1] = (uvIndex + k + 2);
            (*result)[current].attributes[2] = (uvIndex + k + 1);

//            jclogf("uvindex(%d, %d, %d)", (uvIndex + 0), (uvIndex + k + 2), (uvIndex + k + 1));

//! マテリアル番号
            if ((s32) materialNumbers.size() <= i) {
                (*result)[current].material = 0;
//                throw create_exception_t(FbxException, FbxException_MaterialNotFound);
            } else {
                (*result)[current].material = materialNumbers[i];
            }
            current++;
        }
        uvIndex += size;
    }

// 不具合
    if (current != triangleNum) {
        throw create_exception_t(FbxException, FbxException_ArrayIndexBoundsException);
    }

    jclogf("  FbxTriangles(%d)", result->size());
}

}
}
