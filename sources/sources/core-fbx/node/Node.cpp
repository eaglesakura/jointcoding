/*
 * Node.cpp
 *
 *  Created on: 2012/12/14
 */

#include    "jcfbx/node/Node.h"
#include    "jcfbx/node/Mesh.h"

namespace jc {
namespace fbx {

Node::Node(KFbxNode *node, s32 nodeNumber) {
    this->parent = NULL;
    this->nodeNumber = nodeNumber;

    // デフォルトポーズを登録する
    retisterDefaultTake(node);
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
                result.reset(new Node(node, importManager->nextNodeId()));
                break;
            case KFbxNodeAttribute::eMesh:
                jclogf("NodeType(%d = eMesh)", type);
                result = Mesh::createInstance(node, parent, importManager);
                break;
            case KFbxNodeAttribute::eSkeleton:
                // TODO make SkeltonNode
                jclogf("NodeType(%d = eSkelton)", type);
                result.reset(new Node(node, importManager->nextNodeId()));
                break;
            default:
                jclogf("Not Support NodeType(%d)", type);
                result.reset(new Node(node, importManager->nextNodeId()));
                break;
        }
    } else {
        result.reset(new Node(node, importManager->nextNodeId()));
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
