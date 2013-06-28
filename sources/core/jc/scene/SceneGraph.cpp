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
    this->currentPass = -1;
}
SceneGraph::~SceneGraph() {

}

/**
 * 子の数をかぞえる
 */
u32 SceneGraph::getChildNum(const jcboolean recursion) {
    u32 result = children.size();
    std::list<MSceneGraph>::iterator itr = children.begin(), end = children.end();

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
    assert(child.get() != this);

    // 既に自分自身が親だったら何もしない
    if (child->parent == this) {
        return;
    }

    // 親から切り離す
    child->removeFromParent();

    // 関連性を追加する
    child->parent = this;
    children.push_back(child);

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
        std::list<MSceneGraph>::iterator itr = children.begin(), end = children.end();

        // 子を探索する
        while (itr != end) {
            (*itr)->removeChild(child);
            ++itr;
        }

        return;
    }

//    jclogf("detatch (%x)", child.get());
    child->parent = NULL;
    children.remove(child);

    // send message
    child->onSceneUnbinded();
}

/**
 * すべての子を外す
 */
void SceneGraph::removeChildren() {
    std::list<MSceneGraph>::iterator itr = children.begin(), end = children.end();

    while (itr != end) {
        MSceneGraph child = (*itr);

        // リストから外す
        itr = children.erase(itr);
        end = children.end();

        // 親を外す
        child->parent = NULL;

        // send message
        child->onSceneUnbinded();
    }

    if (children.size()) {
        throw create_exception(RuntimeException, "scene have child!!");
    }
}

/**
 * 子を探す
 * 見つからなかったらnullを返す
 */
MSceneGraph SceneGraph::findScene(const scene_id uniqueId) const {
    assert(uniqueId != this->uniqueId);

    std::list<MSceneGraph> *target_list = (std::list<MSceneGraph>*) &children;
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

    std::list<MSceneGraph>::iterator itr = parent->children.begin(), end = parent->children.end();

    while (itr != end) {
        // リストの中から自分を見つけた！
        if ((*itr).get() == this) {
            // 消し去る
            parent->children.erase(itr);

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
    std::list<MSceneGraph>::iterator itr = children.begin(), end = children.end();

    while (itr != end) {
        if ((*itr)->update()) {
            MSceneGraph child = (*itr);

            // リストから排除する
            itr = children.erase(itr);
            end = children.end();

            child->parent = NULL;
            // send message
            child->onSceneUnbinded();
        } else {
            ++itr;
        }
    }

    if (isSelfUpdatePass()) {
        return onSelfUpdate();
    } else {
        return jcfalse;
    }
}

static bool compare_scenegraph(const MSceneGraph a, const MSceneGraph b) {
    return a->getRenderingPriority() < b->getRenderingPriority();
}

/**
 * レンダリングを行う
 */
void SceneGraph::rendering() {
    if (isSelfRenderingPass()) {
        onSelfRendering();
    }

    // 子の処理を行う
    {
        std::list<MSceneGraph> renderingScenes;
        // 元のリストをコピーする
        std::copy(children.begin(), children.end(), std::back_inserter(renderingScenes));

        // sort
        renderingScenes.sort(compare_scenegraph);

        // レンダリング作業
        std::list<MSceneGraph>::iterator itr = renderingScenes.begin(), end = renderingScenes.end();
        while (itr != end) {
            (*itr)->rendering();
            ++itr;
        }
    }

}

}

