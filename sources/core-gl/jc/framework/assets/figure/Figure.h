/*
 * Figure.h
 *
 *  Created on: 2013/05/22
 */

#ifndef JCFWFIGURE_H_
#define JCFWFIGURE_H_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * 1つの3Dモデル=フィギュアを構築する
 */
class Figure: public Object {
    /**
     * 全ノードの一括キャッシュ
     */
    safe_array<FigureNode*> nodes;

public:
    Figure() {
    }

    virtual ~Figure() {
        jclogf("delete Figure(0x%x)", this);
        safe_delete(nodes);
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
        nodes.reserve(nodeNum);
        nodes.zeromemory();

        // 指定数のノードを生成する
        for (s32 i = 0; i < (s32) nodeNum; ++i) {
            nodes[i] = createNode(device, i);
        }
    }

    /**
     * ノードを取得する
     */
    virtual FigureNode* getNode(const u32 index) {
        return nodes[index];
    }

    /**
     * ノードを取得する
     */
    virtual const FigureNode* getNode(const u32 index) const {
        return nodes[index];
    }

    /**
     * ノード数を取得する
     */
    virtual u32 getNodeNum() const {
        return nodes.length;
    }

};

}
}

#endif /* JCFWFIGURE_H_ */
