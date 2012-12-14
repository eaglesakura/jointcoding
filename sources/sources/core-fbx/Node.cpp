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
}

/**
 * 子ノードを追加する
 */
void Node::addChild(MNode node) {

}

/**
 * ノード情報を出力する
 */
MNode Node::createInstance(KFbxNode *node, MNode parent) {
    MNode result;

    if (node->GetNodeAttribute()) {
        const KFbxNodeAttribute::EType type = node->GetNodeAttribute()->GetAttributeType();

        switch (type) {
            case KFbxNodeAttribute::eNull:
                break;
            case KFbxNodeAttribute::eMesh:
                break;
            case KFbxNodeAttribute::eSkeleton:
                break;
            default:
                jclogf("Not Support NodeType = (%d)", type);
                break;
        }
    }

    if (!result) {
        result.reset(new Node(parent.get(), 0));
    }

    return result;
}

}
}
