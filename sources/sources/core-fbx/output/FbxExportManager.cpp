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

    {
        // figure info
        MFigureDataOutputStream stream = createOutputStream(rootNode.get(), "figure.info");

        u32 allNode = rootNode->getAllNodeCount();
        jclogf("all nodes(%d)", allNode);

        // ノード数
        stream->writeU32(rootNode->getAllNodeCount());
    }

    rootNode->serialize(this);
}

}
}
