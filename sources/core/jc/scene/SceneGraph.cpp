/*
 * jcSceneGraph.cpp
 *
 *  Created on: 2012/12/10
 */
#include    "jc/scene/SceneGraph.h"
#include    "jc/system/Exception.h"

namespace jc {

SceneGraph::SceneGraph() {
    this->uniqueId = (scene_id) this;
    this->parent = NULL;
    this->renderingPriority = 1.0f;
    this->parentAhead = jcfalse;
    this->enableMultiPass = jcfalse;
    this->currentPass = -1;
}
SceneGraph::~SceneGraph() {

}

/**
 * 子の数をかぞえる
 */
u32 SceneGraph::getChildNum(const jcboolean recursion) {
    u32 result = childs.size();
    std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();

    if (recursion) {
        while (itr != end) {
            result += (*itr)->getChildNum(recursion);
            ++itr;
        }
    }

    return result;
}

/**
 * 子を追加する
 */
void SceneGraph::pushBackChild(MSceneGraph child) {
    assert(child.get() != NULL);

    // 既に自分自身が親だったら何もしない
    if (child->parent == this) {
        return;
    }

    // 親から切り離す
    child->removeFromParent();

    // 関連性を追加する
    child->parent = this;
    childs.push_back(child);

    // send message
    child->onSceneBinded(this);
}

/**
 * 子を外す
 */
void SceneGraph::removeChild(MSceneGraph child) {
    assert(child.get() != NULL);

    // 違う親が設定されていたら何もしない
    if (child->parent != this) {
        std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();

        // 子を探索する
        while (itr != end) {
            (*itr)->removeChild(child);
            ++itr;
        }

        return;
    }

//    jclogf("detatch (%x)", child.get());
    child->parent = NULL;
    childs.remove(child);

    // send message
    child->onSceneUnbinded();
}

/**
 * すべての子を外す
 */
void SceneGraph::removeChilds() {
    std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();

    while (itr != end) {
        MSceneGraph child = (*itr);

        // リストから外す
        itr = childs.erase(itr);
        end = childs.end();

        // 親を外す
        child->parent = NULL;

        // send message
        child->onSceneUnbinded();
    }

    if (childs.size()) {
        throw create_exception(RuntimeException, "scene have child!!");
    }
}

/**
 * 子を探す
 * 見つからなかったらnullを返す
 */
MSceneGraph SceneGraph::findScene(const scene_id uniqueId) const {
    assert(uniqueId != this->uniqueId);

    std::list<MSceneGraph> *target_list = (std::list<MSceneGraph>*) &childs;
    std::list<MSceneGraph>::iterator itr = target_list->begin(), end = target_list->end();

    while (itr != end) {
        if ((*itr)->getUniqueId() == uniqueId) {
            return *itr;
        } else {
            MSceneGraph result = (*itr)->findScene(uniqueId);
            if (result) {
                return result;
            }
        }
        ++itr;
    }
    return MSceneGraph();
}

/**
 * 親から参照を外す
 */
void SceneGraph::removeFromParent() {
    if (!this->parent) {
        return;
    }

    std::list<MSceneGraph>::iterator itr = parent->childs.begin(), end = parent->childs.end();

    while (itr != end) {
        // リストの中から自分を見つけた！
        if ((*itr).get() == this) {
            // 消し去る
            parent->childs.erase(itr);

            // 親との関連性を切る
            parent = NULL;

            // send message
            onSceneUnbinded();
            return;
        }
        ++itr;
    }

    throw create_exception(RuntimeException, "parent not have this");
}

/**
 * 更新作業を行う
 */
jcboolean SceneGraph::update() {

    jcboolean result = jcfalse;
    // 先に更新する場合は処理する
    if (isParentAhead()) {
        if (isFirstPass() || isMultipassEnable()) {
            result = onSelfUpdate();
        }
    }

    std::list<MSceneGraph>::iterator itr = childs.begin(), end = childs.end();

    while (itr != end) {
        if ((*itr)->update()) {
            MSceneGraph child = (*itr);

            // リストから排除する
            itr = childs.erase(itr);
            end = childs.end();

            child->parent = NULL;
            // send message
            child->onSceneUnbinded();
        } else {
            ++itr;
        }
    }

    if (!isParentAhead()) {
        if (isFirstPass() || isMultipassEnable()) {
            return onSelfUpdate();
        } else {
            return jcfalse;
        }
    } else {
        return result;
    }
}

static bool compare_scenegraph(const MSceneGraph a, const MSceneGraph b) {
    return a->getRenderingPriority() < b->getRenderingPriority();
}

/**
 * レンダリングを行う
 */
void SceneGraph::rendering() {

    if (isParentAhead()) {
        if (isFirstPass() || isMultipassEnable()) {
            onSelfRendering();
        }
    }

    // 子の処理を行う
    {
        std::list<MSceneGraph> renderingScenes;
        // 元のリストをコピーする
        std::copy(childs.begin(), childs.end(), std::back_inserter(renderingScenes));

        // sort
        renderingScenes.sort(compare_scenegraph);

        // レンダリング作業
        std::list<MSceneGraph>::iterator itr = renderingScenes.begin(), end = renderingScenes.end();
        while (itr != end) {
            (*itr)->rendering();
            ++itr;
        }
    }

    if (!isParentAhead()) {
        if (isFirstPass() || isMultipassEnable()) {
            onSelfRendering();
        }
    }
}

}

