/*
 * FbxExportManager.cpp
 *
 *  Created on: 2012/12/25
 */

#include    "jcfbx/output/FigureDataOutputStream.h"
#include    "jcfbx/output/FbxExportManager.h"
#include    "jc/io/FileOutputStream.h"

namespace jc {
namespace fbx {

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

}
}
