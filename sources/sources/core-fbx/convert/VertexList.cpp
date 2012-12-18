/*
 * VertexList.cpp
 *
 *  Created on: 2012/12/18
 */
#include    "jcfbx/convert/VertexList.h"

namespace jc {
namespace fbx {

/**
 * 読み込んだFBXのraw情報をもとにして、頂点一覧を作成する
 */
FbxVertexTable::FbxVertexTable(const VertexContainer &vertices, const IndicesContainer &indices) {
    // マテリアル数だけフラグメントを再構築する
    for (int material_index = 0; material_index < (int)indices.materials.size(); ++material_index) {
        fragments.push_back(MMeshFragment(new MeshFragment()));
    }

    const int polygon_num = (int) indices.polygons.size();
    for (int polygon_index = 0; polygon_index < polygon_num; ++polygon_index) {
        const ConvertedPolygon &polygon = indices.polygons[polygon_index];
        u16 add_indices[] = { (u16) -1, (u16) -1, (u16) -1 };

        for (int k = 0; k < 3; ++k) {
            SimpleVertex v;
            v.position = vertices.positions[polygon.position[k]];
            v.uv = vertices.coords[polygon.attributes[k]];
            v.normal = vertices.normals[polygon.attributes[k]];
            v.weight = vertices.weights[polygon.position[k]];

            add_indices[k] = (u16) registerVertex(v);
        }

        // マテリアルごとに登録するFragmentを拾う
        MMeshFragment targetFragment = fragments[polygon.material];

        // ポリゴンを登録する
        targetFragment->addIndices(this->vertices, add_indices[0], add_indices[1], add_indices[2]);
    }

    jclogf("    converted vertices(%d vertex)", this->vertices.size());
    for (int i = 0; i < (int) fragments.size(); ++i) {
        jclogf("      material[%d] (%d faces %d bones)", i, fragments[i]->getPolygonCount(), fragments[i]->getBoneCount());
    }
#if 0
    for (s32 i = 0; i < triangles.length(); ++i) {
        for (s32 k = 0; k < 3; ++k) {
            SmartPtr < FbxVertex > v = createVertex(&triangles[i], k);

            //! 頂点を登録する
            s32 index = fvt.regist(v);
            indices[triangles[i].material].push_back(index);
            ++ptr;
        }
    }

    //! 出力用の頂点バッファを作成する
    {
        outVertices = new FbxVertex[fvt.vertices.size()];
        for (s32 i = 0; i < fvt.vertices.size(); ++i) {
            outVertices[i] = *fvt.vertices[i];
        }
    }

    //! 出力用のインデックスバッファを作成する
    for (s32 i = 0; i < indices.size(); ++i) {
        s32 indexLength = indices[i].size();
        materials[i].indices = new s32[indexLength];

        for (s32 k = 0; k < indexLength; ++k) {
            materials[i].indices[k] = indices[i][k];
        }
    }
#endif
}

FbxVertexTable::~FbxVertexTable() {

}

s32 FbxVertexTable::registerVertex(const SimpleVertex &vertex) {
    for (u32 i = 0; i < vertices.size(); ++i) {
        if (vertex.equals(vertices[i])) {
            //! 頂点が一致した
//            jclogf("    vertex cached(%d)", i);
            return i;
        }
    }

    vertices.push_back(vertex);
//    jclogf("    vertex created(%d)", vertices.size() - 1);
    return (s32) (vertices.size() - 1);
}

}
}
