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

namespace jc {
namespace fbx {

class FbxVertexTable {
public:

    /**
     * 読み込んだFBXのraw情報をもとにして、頂点一覧を作成する
     */
    FbxVertexTable(const VertexContainer &vertices, const IndicesContainer &indices);

    ~FbxVertexTable();

};

}
}

#endif /* VERTEXLIST_H_ */
