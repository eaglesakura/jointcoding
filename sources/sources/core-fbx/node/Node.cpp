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
    this->name = node->GetName();

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
 * 出力を行う
 */
void Node::serialize(FbxExportManager *exportManager) {
    MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("node%03d.node", getNodeNumber()));

    // ノードの基本情報
    {
        stream->writeString(getName()); // ノード名
        stream->writeU16(getNodeNumber()); // ノード番号
        stream->writeU16(getNodeType()); // ノード種別

        // 子ノード番号を書き出す
        stream->writeU16((u16) childs.size());
        for (u32 i = 0; i < childs.size(); ++i) {
            stream->writeU16(childs[i]->getNodeNumber());
        }
    }

    // transform
    {
        stream->writeVector3(transform.scale);

        {
            stream->writeS8(transform.rotateType);
            stream->writeVector3(transform.rotate);

            // vec4としてreadできるように、4byte詰め物をする
            stream->writeS32(0);
        }
        stream->writeVector3(transform.translate);
    }

    // 子ノードを出力
    for (u32 i = 0; i < childs.size(); ++i) {
        childs[i]->serialize(exportManager);
    }
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

/**
 * thisを頂点として、管理しているノード数合計を返す。
 * thisを持つため、>=1は必ず返却されることになる。
 */
u32 Node::getAllNodeCount() const {
    u32 result = 1;

    for (u32 i = 0; i < childs.size(); ++i) {
        result += childs[i]->getAllNodeCount();
    }

    return result;
}

}
}
