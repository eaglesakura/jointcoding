/*
 * jcSceneGraph.h
 *
 *  Created on: 2012/12/10
 */

#ifndef JCSCENEGRAPH_H_
#define JCSCENEGRAPH_H_

#include    "jc/system/Macro.h"
#include    "jc/model/Object.h"
#include    "jc/mem/SmartPtr.h"
#include    <list>

namespace jc {

class SceneGraph;
typedef jc_sp<SceneGraph> MSceneGraph;

typedef u64 scene_id;

class SceneGraph: public Object {
    /**
     * 識別子
     * デフォルトはランダムで設定される
     */
    scene_id uniqueId;

    /**
     * 子参照
     */
    std::list<MSceneGraph> childs;

    /**
     * 親参照
     */
    SceneGraph* parent;

    /**
     * レンダリング優先順
     * 標準は1.0で、小さい値のシーンを優先的に描画する
     */
    float renderingPriority;

public:
    SceneGraph();
    virtual ~SceneGraph();

    virtual scene_id getUniqueId() const {
        return uniqueId;
    }

    virtual void setUniqueId(const scene_id set) {
        uniqueId = set;
    }

    /**
     * 子を追加する
     */
    virtual void pushBackChild(MSceneGraph child);

    /**
     * 子を外す
     */
    virtual void removeChild(MSceneGraph child);

    /**
     * すべての子を外す
     */
    virtual void removeChilds();

    /**
     * 子を探す
     * 見つからなかったらnullを返す
     */
    virtual MSceneGraph findScene(const scene_id uniqueId) const;

    /**
     * レンダリング優先度を取得する
     */
    virtual float getRenderingPriority() const {
        return renderingPriority;
    }

    /**
     * 親から参照を外す
     */
    virtual void removeFromParent();

    /**
     * 更新作業を行う
     * trueを返すと「処理完了」とみなして排除する
     */
    virtual jcboolean update();

    /**
     * レンダリングを行う
     * 子（プライオリティ順） -> 親の順番で描画される
     */
    virtual void rendering();

    /**
     * 親シーンに統合されたタイミングで呼び出される
     */
    virtual void onSceneBinded(SceneGraph *newParent) {
    }

    /**
     * 親シーンから外されたタイミングで呼び出される
     */
    virtual void onSceneUnbinded() {
    }

    /**
     * 自分自身の更新作業を行う
     */
    virtual jcboolean onSelfUpdate() {
        return jcfalse;
    }

    /**
     * 自分自身のレンダリングを行う
     */
    virtual void onSelfRendering() {
    }
};

}

#endif /* JCSCENEGRAPH_H_ */
