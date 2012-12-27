/*
 * Node.h
 *
 *  Created on: 2012/12/14
 */

#ifndef FBX_NODE_H_
#define FBX_NODE_H_

#include    "jointcoding-fbx.h"
#include    "jc/math/EulerTransform.h"
#include    "jc/graphics/figure/Figure.h"
#include    <vector>

#include    "jcfbx/output/FbxExportManager.h"

namespace jc {
namespace fbx {

class Node;
typedef jc_sp<Node> MNode;

/**
 * FBXノード情報の基底クラス。
 *
 * Node
 *   L Frame
 *   L Null
 *   L Bone
 */
class Node {
    /**
     * 子ノード
     */
    std::vector<MNode> childs;

    /**
     * 親ノード
     */
    Node *parent;

    /**
     * ノード名
     */
    String name;

    /**
     * ノード番号
     * 通常、通し番号
     */
    s32 nodeNumber;

protected:
    /**
     * 位置情報
     */
    EulerTransform transform;

    Node(KFbxNode *node, s32 nodeNumber);

    /**
     * 初期姿勢を登録する
     */
    virtual void retisterDefaultTake(KFbxNode *node);

    /**
     * ノード状態をログ出力する
     */
    virtual void printNodeInfo(const s32 indent);

public:
    virtual ~Node();

    /**
     * ノードの番号を取得する
     */
    virtual s32 getNodeNumber() const {
        return nodeNumber;
    }

    virtual EulerTransform& getTransform() {
        return transform;
    }

    /**
     * 子ノードを追加する
     */
    virtual void addChild(MNode node);

    /**
     * 基底クラスはNULLノードとして扱う。
     */
    virtual NodeType_e getNodeType() const {
        return NodeType_NULL;
    }

    /**
     * ノード名を取得する
     */
    virtual String getName() const {
        return name;
    }

    /**
     * thisを頂点として、管理しているノード数合計を返す。
     * thisを持つため、>=1は必ず返却されることになる。
     */
    virtual u32 getAllNodeCount() const;

    /**
     * 出力を行う
     */
    virtual void serialize(FbxExportManager *exportManager);

    /**
     * ノード情報を出力する
     */
    static MNode createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager);
};

}
}

#endif /* NODE_H_ */
