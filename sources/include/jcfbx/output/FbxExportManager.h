/*
 * FbxExportManager.h
 *
 *  Created on: 2012/12/20
 */

#ifndef FBXEXPORTMANAGER_H_
#define FBXEXPORTMANAGER_H_

#include    "jc/graphics/figure/data/FigureDataOutputStream.h"

namespace jc {
namespace fbx {

class Node;

class FbxExportManager: public Object {
public:
    FbxExportManager();

    virtual ~FbxExportManager();

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

    /**
     * 全ノードをシリアライズする。
     */
    virtual void serialize(jc_sp<Node> rootNode);
};

}
}

#endif /* FBXEXPORTMANAGER_H_ */
