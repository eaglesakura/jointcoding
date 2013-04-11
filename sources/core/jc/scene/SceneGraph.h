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
     * 親参照
     */
    SceneGraph* parent;

    /**
     * レンダリング優先順
     * 標準は1.0で、小さい値のシーンを優先的に描画する
     */
    float renderingPriority;

    /**
     * 親を先に実行する
     */
    jcboolean parentAhead;

    /**
     * マルチパス動作を許可する
     * デフォルトOFF
     */
    jcboolean enableMultiPass;

    /**
     * 現在の処理パス
     * 複数パスでのレンダリング・更新用
     * デフォルトは-1
     */
    s32 currentPass;

protected:
    /**
     * 子参照
     */
    std::list<MSceneGraph> childs;

    /**
     * 現在の処理パスを取得する
     */
    virtual s32 getCurrentPass() const {
        return currentPass;
    }

    /**
     * マルチパスの動作を許可する場合true
     * falseの場合、passが0以下の場合のみonSelfUpdate/onSelfRenderingが動作する
     */
    virtual void setMultipassEnable(const jcboolean set) {
        enableMultiPass = set;
    }

    virtual jcboolean isMultipassEnable() const {
        return enableMultiPass;
    }

    /**
     * 最初のパスならtrue
     */
    virtual s32 isFirstPass() const {
        return currentPass <= 0;
    }
public:
    SceneGraph();
    virtual ~SceneGraph();

    /**
     * パス処理を開始する場合に呼び出す
     *
     * scene->beginPass(0);
     * scene->beginPass(1);
     * scene->beginPass(2);
     * scene->endPass();
     */
    virtual void beginPass(const s32 pass) {
        assert(pass >= 0);
        currentPass = pass;
        if (isParentAhead()) {
            onBeginPass(pass);
        }

        std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();
        while (itr != end) {
            (*itr)->beginPass(pass);
            ++itr;
        }

        if (!isParentAhead()) {
            onBeginPass(pass);
        }
    }

    /**
     * パス動作を完了する
     */
    virtual void endPass() {
        currentPass = -1;
        if (isParentAhead()) {
            onEndPass();
        }

        std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();
        while (itr != end) {
            (*itr)->endPass();
            ++itr;
        }

        if (!isParentAhead()) {
            onEndPass();
        }
    }

    virtual scene_id getUniqueId() const {
        return uniqueId;
    }

    virtual u32 getChildrenNum() const {
        return childs.size();
    }

    virtual jcboolean isParentAhead() const {
        return parentAhead;
    }

    /**
     * 親を先に更新・描画を行う場合trueを設定する。
     * デフォルトfalse = 子を優先して処理する
     */
    virtual void setParentAhead(const jcboolean set) {
        parentAhead = set;
    }

    /**
     * 子の数をかぞえる
     * trueを設定した場合、再帰的に子の数をかぞえる
     */
    virtual u32 getChildNum(const jcboolean recursion);

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
     * 子を探して適当なクラスへキャストする
     */
    template<typename T>
    jc_sp<T> findSceneTo(const scene_id uniqueId) const {
        MSceneGraph result = findScene(uniqueId);
        return jc_sp<T>( dynamic_cast<T*>(result.get()) );
    }

    /**
     * レンダリング優先度を取得する
     */
    virtual float getRenderingPriority() const {
        return renderingPriority;
    }

    /**
     * レンダリング優先度を取得する
     * priorityが大きい順に描画する
     */
    virtual void setRenderingPriority(const float set) {
        this->renderingPriority = set;
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

protected:

    /**
     * 現在のパスが開始された
     */
    virtual void onBeginPass( const s32 pass) {
    }

    /**
     * パス動作が終了した
     */
    virtual void onEndPass() {
    }

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
     * trueを返すと「処理完了」とみなして排除する
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
