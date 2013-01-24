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
}
SceneGraph::~SceneGraph() {

}

/**
 * 子を追加する
 */
void SceneGraph::pushBackChild(MSceneGraph child) {
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
    // 違う親が設定されていたら何もしない
    if (child->parent != this) {
        return;
    }

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

    return onSelfUpdate();
}

static bool compare_scenegraph(const MSceneGraph a, const MSceneGraph b) {
    return a->getRenderingPriority() < b->getRenderingPriority();
}

/**
 * レンダリングを行う
 */
void SceneGraph::rendering() {
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

    onSelfRendering();
}

}

