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
    this->fbxNode = node;
    this->fbxUniqueId = node->GetUniqueID();

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
            transform.rotate.set((float) v[0], (float) v[1], (float) v[2], 0);
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
 * アニメーションを登録する
 */
void Node::registerAnimations() {
    KFbxScene *scene = fbxNode->GetScene();

    // アニメーション名を取得する
    FbxArray<FbxString*> animationNames;
    scene->FillAnimStackNameArray(animationNames);

    KTime period;
    //! 1フレーム単位の時間を取得
    period.SetTime(0, 0, 0, 1, 0, scene->GetGlobalSettings().GetTimeMode());

    if (animationNames.Size() == 0) {
        jclog("animation not found...");
        return;
    }

    KFbxTakeInfo *info = scene->GetTakeInfo(animationNames[0]->Buffer());

    if (info) {
        KTime start = info->mLocalTimeSpan.GetStart();
        KTime end = info->mLocalTimeSpan.GetStop();

        s32 startFrame = (s32) (start.Get() / period.Get());
        s32 endFrame = (s32) (end.Get() / period.Get());

        jclogf("    Node(%s) Frame %d -> %d", name.c_str(), startFrame, endFrame);

        KFbxAnimEvaluator *evalutor = scene->GetEvaluator();

        u32 translate_keys = 0;
        u32 rotate_keys = 0;
        u32 scale_keys = 0;

        for (s32 i = startFrame; i < endFrame; ++i) {
            KFbxVector4 translate = evalutor->GetNodeLocalTranslation(fbxNode, period * i);
            KFbxVector4 rotate = evalutor->GetNodeLocalRotation(fbxNode, period * i);
            KFbxVector4 scale = evalutor->GetNodeLocalScaling(fbxNode, period * i);

            translate_keys += animator.addTranslateAnimation(TranslateKey(i, Vector3f(translate[0], translate[1], translate[2])));
            rotate_keys += animator.addRotateAnimation(RotateKey(i, Vector4f(rotate[0], rotate[1], rotate[2], 0)));
            scale_keys += animator.addScaleAnimation(ScaleKey(i, Vector3f(scale[0], scale[1], scale[2])));
        }

        jclogf("      node keys(T %d, R %d, S %d)", translate_keys, rotate_keys, scale_keys);
    }
    FbxArrayDelete(animationNames);

    // 子も登録する
    for (u32 i = 0; i < (u32) childs.size(); ++i) {
        childs[i]->registerAnimations();
    }
}

/**
 * 出力を行う
 */
void Node::serialize(FbxExportManager *exportManager) {

    // ノードの基本情報
    {
        MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("node%03d.node", getNodeNumber()));

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
                stream->writeVector3(*((Vector3f*) &transform.rotate));

                // vec4としてreadできるように、4byte詰め物をする
                stream->writeS32(0);
            }
            stream->writeVector3(transform.translate);
        }
    }

    // animation
    if (!animator.empty()) {
        {
            MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("node%03d.anim_t", getNodeNumber()));
            const std::vector<TranslateKey> &keys = animator.getTranslates();

            // 割り当てられたノード名
            stream->writeString(name);

            // キーフレーム数
            stream->writeU16((u16) keys.size());

            // 全キー
            for (size_t i = 0; i < keys.size(); ++i) {
                const TranslateKey &key = keys[i];

                stream->writeU16((u16) key.frame);
                stream->writeVector3(key.value);
            }
        }
        {
            MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("node%03d.anim_r", getNodeNumber()));
            const std::vector<RotateKey> &keys = animator.getRotates();

            // 割り当てられたノード名
            stream->writeString(name);

            // キーフレーム数
            stream->writeU16((u16) keys.size());

            // 全キー
            for (size_t i = 0; i < keys.size(); ++i) {
                const RotateKey &key = keys[i];

                stream->writeU16((u16) key.frame);

                // rotate = vector4
                stream->writeVector3(Vector3f(key.value.x, key.value.y, key.value.z));
                stream->writeFixed32(0);
            }
        }
        {
            MFigureDataOutputStream stream = exportManager->createOutputStream(this, String::format("node%03d.anim_s", getNodeNumber()));
            const std::vector<ScaleKey> &keys = animator.getScales();

            // 割り当てられたノード名
            stream->writeString(name);

            // キーフレーム数
            stream->writeU16((u16) keys.size());

            // 全キー
            for (size_t i = 0; i < keys.size(); ++i) {
                const ScaleKey &key = keys[i];

                stream->writeU16((u16) key.frame);
                stream->writeVector3(key.value);
            }
        }
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
            case KFbxNodeAttribute::eSkeleton:
                //
                jclogf("NodeType(%d = DefaultNode)", type);
                result.reset(new Node(node, importManager->nextNodeId()));
                break;
            case KFbxNodeAttribute::eMesh:
                jclogf("NodeType(%d = eMesh)", type);
                result = Mesh::createInstance(node, parent, importManager);
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
