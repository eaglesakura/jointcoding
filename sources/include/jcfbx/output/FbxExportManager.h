/*
 * FbxExportManager.h
 *
 *  Created on: 2012/12/20
 */

#ifndef FBXEXPORTMANAGER_H_
#define FBXEXPORTMANAGER_H_

#include    "jcfbx/output/FigureDataOutputStream.h"

namespace jc {
namespace fbx {

class Node;

class FbxExportManager {
public:

    /**
     * データ出力用のストリームを作成する
     *
     * @param current 出力用ノード
     * @param file_ext ファイル拡張子
     */
    MFigureDataOutputStream createOutputStream(Node *current, const charactor *fileName);

    /**
     * データ出力用のストリームを作成する
     */
    MFigureDataOutputStream createOutputStream(Node *current, const String &fileName) {
        return createOutputStream(current, fileName.c_str());
    }
};

}
}

#endif /* FBXEXPORTMANAGER_H_ */
