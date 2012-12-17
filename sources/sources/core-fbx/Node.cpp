/*
 * Node.cpp
 *
 *  Created on: 2012/12/14
 */

#include    "jcfbx/Node.h"

namespace jc {
namespace fbx {

Node::Node(Node* parent, s32 nodeNumber) {
    this->parent = parent;
    this->nodeNumber = nodeNumber;
}

Node::~Node() {

}

/**
 * 初期姿勢を登録する
 */
void Node::retisterDefaultTake(KFbxNode *node) {
    this->name = node->GetName();

    // 基本姿勢
    {
        // 位置情報
        {
            FbxDouble3 v = node->LclTranslation.Get();
            transform.translate.set((float) v[0], (float) v[1], (float) v[2]);
        }
        // 回転情報
        {
            FbxDouble3 v = node->LclRotation.Get();
            transform.rotate.set((float) v[0], (float) v[1], (float) v[2]);
        }
        // 基本スケーリング
        {
            FbxDouble3 v = node->LclScaling.Get();
            transform.scale.set((float) v[0], (float) v[1], (float) v[2]);
        }
    }

    // ノード情報の出力
    printNodeInfo(0);
}

/**
 * ノード状態をログ出力する
 */
void Node::printNodeInfo(const s32 indent) {
    String indent_str = "";
    for (int i = 0; i < indent; ++i) {
        indent_str += "   ";
    }

    charactor const *indents = indent_str.c_str();

    jclogf("%s------------- Node Infomation -------------", indents);
    jclogf("%sNodeName(%s)", indents, name.c_str());
    {
        jclogf("%sTransform", indents);
        jclogf("%s--translate (%.3f, %.3f, %.3f)", indents, transform.translate.x, transform.translate.y, transform.translate.z);
        jclogf("%s--rotate    (%.3f, %.3f, %.3f)", indents, transform.rotate.x, transform.rotate.y, transform.rotate.z);
        jclogf("%s--scale     (%.3f, %.3f, %.3f)", indents, transform.scale.x, transform.scale.y, transform.scale.z);
    }
    jclogf("%s-------------------------------------------", indents);

}

/**
 * 子ノードを追加する
 */
void Node::addChild(MNode node) {
    if (node->parent == this) {
        return;
    }

    if (node->parent) {
        throw create_exception_t(FbxException, FbxException_NodeHasParent);
    }

    node->parent = this;
    childs.push_back(node);
}

/**
 * ノード情報を出力する
 */
MNode Node::createInstance(KFbxNode *node, MNode parent, FbxImportManager *importManager) {
    MNode result;

    if (node->GetNodeAttribute()) {
        const KFbxNodeAttribute::EType type = node->GetNodeAttribute()->GetAttributeType();

        jclogf("NodeType(%d)", type);
        switch (type) {
            case KFbxNodeAttribute::eNull:
                // TODO make NullNode
                jclogf("NodeType(%d = eNull)", type);
                break;
            case KFbxNodeAttribute::eMesh:
                // TODO make MeshNode
                jclogf("NodeType(%d = eMesh)", type);
                break;
            case KFbxNodeAttribute::eSkeleton:
                // TODO make SkeltonNode
                jclogf("NodeType(%d = eSkelton)", type);
                break;
            default:
                jclogf("Not Support NodeType(%d)", type);
                break;
        }
    }

    if (!result) {
        result.reset(new Node(parent.get(), importManager->nextNodeId()));
        result->retisterDefaultTake(node);
    }

    jclogf("childs(%d Nodes)", node->GetChildCount());
    for (int i = 0; i < node->GetChildCount(); ++i) {
        MNode child = createInstance(node->GetChild(i), result, importManager);
        result->addChild(child);
    }

    assert(result.get() != NULL);
    return result;
}

}
}
