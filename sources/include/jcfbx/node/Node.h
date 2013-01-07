/*
 * Node.h
 *
 *  Created on: 2012/12/14
 */

#ifndef FBX_NODE_H_
#define FBX_NODE_H_

#include    "jointcoding-fbx.h"
#include    "jc/math/Transform.h"
#include    "jc/graphics/figure/Figure.h"
#include    "jc/graphics/figure/animator/Animator.h"
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

    /**
     * FBXが割り当てた一意のID
     */
    u64 fbxUniqueId;

    /**
     * アニメーション管理クラス
     */
    Animator animator;
protected:
    /**
     * FBX側のノード情報
     */
    KFbxNode *fbxNode;

protected:
    /**
     * 位置情報
     */
    Transform transform;

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

    virtual Transform& getTransform() {
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
     * 大本の親ノードを取得する
     */
    virtual Node* getRootNode() const {
        Node *result = (Node*) this;

        while (result->parent) {
            result = result->parent;
        }

        return result;
    }
private:
    virtual Node* findNodeFromFbxUniqueId(const Node *current, const u64 fbxUnique) const {
        if (current->fbxUniqueId == fbxUnique) {
            return (Node*) current;
        }

        for (u32 i = 0; i < current->childs.size(); ++i) {
            Node *node = findNodeFromFbxUniqueId(current->childs[i].get(), fbxUnique);
            if (node) {
                return node;
            }
        }

        // 見つからなかった
        return NULL;
    }

public:
    /**
     * FBX UniqueIDを指定して取得する。
     * 親も含めてツリー全体を探索する。
     * 見つからなかった場合はNULLを返す。
     */
    virtual Node* findNodeFromFbxUniqueId(const u64 fbxUnique) const {
        return findNodeFromFbxUniqueId(getRootNode(), fbxUnique);
    }
public:
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
     * ボーン階層を再構築する
     */
    virtual void registerBones() {
        for (u32 i = 0; i < childs.size(); ++i) {
            childs[i]->registerBones();
        }
    }

    /**
     * アニメーションを登録する
     */
    virtual void registerAnimations();

    /**
     * ノード情報を出力する
     */
    static MNode createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager);
};

}
}

#endif /* NODE_H_ */
