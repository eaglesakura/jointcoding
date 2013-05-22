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

class Figure: public Object {
    /**
     * 全ノードの一括キャッシュ
     */
    handle_table<FigureNode*> nodes;

    static void node_callback(const HandleCallback_e callback_type, handle_table<FigureNode*>* pHandleTable_this, FigureNode** pValues, handle_meta *pMetaHeader, const u32 objects) {
        switch (callback_type) {
            case HandleCallback_Allocated:
                zeromemory(pValues, sizeof(FigureNode*) * objects);
                break;
        }
    }

public:
    Figure() {
        nodes.setCallback((handletable_callback) Figure::node_callback);
    }

    virtual ~Figure() {
        // ノードの個々を削除する
        safe_delete(nodes);
    }

public:
    /* for loader */

    /**
     * 指定したインデックスのノードを生成する
     */
    virtual FigureNode* createNode(MDevice device, const s32 index) {
        return new FigureNode();
    }
    /**
     * ノードテーブルを初期化する
     */
    virtual void initializeNodes(MDevice device, const u32 nodeNum) {
        nodes.reserve(nodeNum);
    }
};

}
}

#endif /* JCFWFIGURE_H_ */
