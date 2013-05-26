/*
 * Figure.h
 *
 *  Created on: 2013/05/22
 */

#ifndef JCFWFIGURE_H_
#define JCFWFIGURE_H_

#include    "jc/framework/assets/figure/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * 1つの3Dモデル=フィギュアを構築する
 */
class Figure: public Object {
    /**
     * 全ノードの一括キャッシュ
     */
    jc_sp<FigureNodeTable> nodes;

    /**
     * 頭となるノード
     */
    FigureNodeHandle rootNode;

    static void node_callback(const HandleCallback_e callback_type, FigureNodeTable* pHandleTable_this, FigureNode** pValues, handle_meta *pMetaHeader, const u32 objects) {
        switch (callback_type) {
            case HandleCallback_Allocated: {
                zeromemory(pValues, sizeof(FigureNode*) * objects);
            }
            break;

            case HandleCallback_Destroy: {
                // デストラクタが起動された
                for (u32 i = 0; i < objects; ++i) {
                    // 解放を行う
                    SAFE_DELETE(pValues[i]);
                }
            }
            break;
        }
    }

public:
    Figure() {
        nodes.reset(new FigureNodeTable());
        nodes->setCallback((handletable_callback) Figure::node_callback);
        nodes->setUserdata((void*) this);
    }

    virtual ~Figure() {
        jclogf("delete Figure(0x%x)", this);
    }

protected:

    /**
     * 指定したインデックスのノードを生成する
     */
    virtual FigureNode* createNode(MDevice device, const s32 index) {
        return new FigureNode(index);
    }

public:
    /* for loader */

    /**
     * ノードテーブルを初期化する
     */
    virtual void initializeNodes(MDevice device, const u32 nodeNum) {
        // 指定数のノードを確保する
        nodes->reserve(nodeNum);

        // 指定数のノードを生成する
        for (s32 i = 0; i < (s32) nodeNum; ++i) {
            nodes->set(i, createNode(device, i));
        }

        // ルートノードを指定する
        rootNode.reset(nodes);
    }

    /**
     * ノードを取得する
     */
    virtual FigureNode* getNode( const u32 index) {
        assert(index < nodes->getTableNum());
        return nodes->get(index);
    }

    /**
     * ノード数を取得する
     */
    virtual u32 getNodeNum() const {
        return nodes->getTableNum();
    }

    /**
     * ハンドルを取得する
     */
    virtual FigureNodeHandle getNodeHandle(const u32 index) {
        FigureNodeHandle result;
        result.reset(handle_data((s32)index), nodes);
        return result;
    }
};

}
}

#endif /* JCFWFIGURE_H_ */
