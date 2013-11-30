/*
 * ApplicationFragmentController.hpp
 *
 *  Created on: 2013/11/30
 */
#ifndef APPLICATIONFRAGMENTCONTROLLER_HPP_
#define APPLICATIONFRAGMENTCONTROLLER_HPP_

#include    "jointcoding.h"
#include    <list>
// frameworks
#include    "jc/framework/fragment/IApplicationFragment.hpp"

namespace jc {
namespace gl {

/**
 * フラグメント管理を行う
 */
class ApplicationFragmentController: public Object {

    typedef typename std::list<SApplicationFragment, StlAllocator<SApplicationFragment> > fragment_container;

    jcmutex transactionLock;

//    jcmutex lifecycleLock;

    /**
     * 管理中のフラグメント
     */
    fragment_container fragments;

    /**
     * フラグメントの追加リクエスト
     */
    fragment_container requestAddFragment;

    /**
     * フラグメントの削除リクエスト
     */
    fragment_container requestRemoveFragment;

    JointApplicationBase *application;

protected:
    virtual void onAddFragment(SApplicationFragment fragment) {
        assert(fragment);

        fragment->onAttach(this);
        fragment->onResume();

        fragments.push_back(fragment);
    }

    virtual void onRemoveFragment(SApplicationFragment fragment) {
        assert(fragment);

        fragment->onPause();
        fragment->onDetatch();

        fragments.remove(fragment);
    }

public:
    ApplicationFragmentController(JointApplicationBase *app) {
        this->application = app;
    }

    virtual ~ApplicationFragmentController() {
    }

    /**
     * フラグメント追加を行う
     */
    virtual void addFragment(SApplicationFragment fragment) {
        MutexLock lock(transactionLock);

        requestAddFragment.push_back(fragment);
    }

    /**
     * フラグメントの削除を行う
     */
    virtual void removeFragment(SApplicationFragment fragment) {
        MutexLock lock(transactionLock);

        requestRemoveFragment.push_back(fragment);
    }

    /**
     * 追加・削除リクエストの処理を行う
     */
    virtual void commit() {
        MutexLock lock(transactionLock);

        // 追加リストを処理する
        if (!requestAddFragment.empty()) {
            fragment_container::iterator itr = requestAddFragment.begin(), end = requestAddFragment.end();
            while (itr != end) {
                onAddFragment(*itr);
                ++itr;
            }

            requestAddFragment.clear();
        }

        // 削除リストを処理する
        if (!requestRemoveFragment.empty()) {
            fragment_container::iterator itr = requestRemoveFragment.begin(), end = requestRemoveFragment.end();
            while (itr != end) {
                onRemoveFragment(*itr);
                ++itr;
            }

            requestRemoveFragment.clear();
        }
    }

    /**
     * フラグメント復旧を行う
     */
    virtual void onSurfaceResized(const u32 width, const u32 height) {
        fragment_container::iterator itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onSurfaceResized(width, height);
            ++itr;
        }
    }

    /**
     * フラグメント復旧を行う
     */
    virtual void onResume() {
        fragment_container::iterator itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onResume();
            ++itr;
        }
    }

    /**
     * 毎フレーム更新を行う
     */
    virtual void onUpdate() {
        fragment_container::iterator itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onUpdate();
            ++itr;
        }

    }

    /**
     * フラグメント休止を行う
     */
    virtual void onPause() {
        fragment_container::iterator itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onPause();
            ++itr;
        }
    }

    /**
     * 解放処理を行う
     */
    virtual void onDestroy() {
        fragment_container::iterator itr = fragments.begin(), end = fragments.end();

        while (itr != end) {
            (*itr)->onDestroy();
            ++itr;
        }
    }

    virtual JointApplicationBase* getApplication() const {
        return application;
    }

    /**
     * IDを指定してFragmentを取得する
     */
    virtual SApplicationFragment findFragmentById(const s32 id) const {
        MutexLock lock(transactionLock);

        fragment_container::const_iterator itr = fragments.begin(), end = fragments.end();
        while (itr != end) {
            if ((*itr)->getId() == id) {
                return (*itr);
            }
            ++itr;
        }
        return SApplicationFragment();
    }

    /**
     * タグを指定してFragmentを取得する
     */
    virtual SApplicationFragment findFragmentByTag(const String tag) {
        MutexLock lock(transactionLock);

        fragment_container::const_iterator itr = fragments.begin(), end = fragments.end();
        while (itr != end) {
            if ((*itr)->getTag() == tag) {
                return (*itr);
            }
            ++itr;
        }
        return SApplicationFragment();
    }
};

typedef jc_sp<ApplicationFragmentController> MApplicationFragmentController;

}
}

#endif /* APPLICATIONFRAGMENTCONTROLLER_HPP_ */
