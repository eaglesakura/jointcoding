/*
 * FbxExportManager.cpp
 *
 *  Created on: 2012/12/25
 */

#include    "jc/graphics/figure/data/FigureDataOutputStream.h"
#include    "jc/io/FileOutputStream.h"

#include    "jcfbx/node/Node.h"
#include    "jcfbx/output/FbxExportManager.h"

namespace jc {
namespace fbx {

FbxExportManager::FbxExportManager() {

}

FbxExportManager::~FbxExportManager() {

}

/**
 * データ出力用のストリームを作成する
 *
 * @param current 出力用ノード
 * @param file_ext ファイル拡張子
 */
MFigureDataOutputStream FbxExportManager::createOutputStream(Node *current, const charactor *fileName) {

    jcboolean completed;
    MOutputStream os(new FileOutputStream(fileName, &completed));
    if (!completed) {
        throw create_exception(FileNotFoundException, "output stream cannot open!!");
    }

    MFigureDataOutputStream result(new FigureDataOutputStream(os));
    result->initialize(FigureDataOutputStream::ModelerType_3dsMax);
    return result;
}

/**
 * 全ノードをシリアライズする。
 */
void FbxExportManager::serialize(MNode rootNode) {

    // 事前検証
    {
        // 頂点数がunsigned shortの範囲を超えたら、エラーとして扱う
        if (rootNode->getAllVertexNum() > (u32) 0xFFFF) {
            jclogf("over vertex num(unsigned short) vertices(%d)", rootNode->getAllVertexNum());
            return;
        }
    }

    {
        // figure info
        MFigureDataOutputStream stream = createOutputStream(rootNode.get(), "figure.info");

        // ノード数
        stream->writeU32(rootNode->getAllNodeCount());
        jclogf("all nodes(%d)", rootNode->getAllNodeCount());

        // 頂点数
        stream->writeU32(rootNode->getAllVertexNum());
        jclogf("all vertices(%d)", rootNode->getAllVertexNum());

        // インデックス数
        stream->writeU32(rootNode->getAllIndicesNum());
        jclogf("all indices(%d)", rootNode->getAllIndicesNum());

        // マテリアル数を数える
        {
            std::map<String, MFigureMaterial> materials;
            rootNode->getAllMaterials(&materials);

            jclogf("all materials(%d)", materials.size());

            // マテリアル数
            stream->writeU32(materials.size());
        }

    }

    // 全体をシリアライズする
    rootNode->serialize(this);
}

}
}
