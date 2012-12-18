/*
 * VertexList.h
 *
 *  Created on: 2012/12/18
 */

#ifndef JCFBXVERTEXLIST_H_
#define JCFBXVERTEXLIST_H_

#include    "jointcoding-fbx.h"
#include    "jcfbx/attribute/VertexContainer.h"
#include    "jcfbx/attribute/IndicesContainer.h"
#include    "jcfbx/convert/FbxVetex.h"
#include    "jcfbx/convert/MeshFragment.h"

namespace jc {
namespace fbx {

class FbxVertexTable {

    /**
     * 頂点を登録し、インデックスを返す
     */
    s32 registerVertex(const SimpleVertex &vertex);
public:
    /**
     * 登録された頂点一覧
     */
    std::vector<SimpleVertex> vertices;

    /**
     * マテリアル断片
     * framents[material_index]で、マテリアルごとのフラグメントが手に入る。
     */
    std::vector<MMeshFragment> fragments;

    /**
     * 読み込んだFBXのraw情報をもとにして、頂点一覧を作成する
     */
    FbxVertexTable(const VertexContainer &vertices, const IndicesContainer &indices);

    ~FbxVertexTable();
};

}
}

#endif /* VERTEXLIST_H_ */
