/*
 * IApplicationFragment.hpp
 *
 *  Created on: 2013/11/30
 */
#ifndef IAPPLICATIONFRAGMENT_HPP_
#define IAPPLICATIONFRAGMENT_HPP_

#include    "jointcoding.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/gl/context/RenderingContext.hpp"

namespace jc {
namespace gl {

class JointApplicationBase;
class ApplicationFragmentController;

/**
 * アプリの子要素としてコントローラーを追加する
 */
class IApplicationFragment: public Object {

    ApplicationFragmentController *controller;

    s32 id;

    String tag;

protected:
    IApplicationFragment() {
        controller = NULL;
        id = 0;
    }
public:
    virtual ~IApplicationFragment() {
    }

    virtual void onAttach(ApplicationFragmentController *ctrl) {
        this->controller = ctrl;
    }

    /**
     * サーフェイスサイズが変更された
     */
    virtual void onSurfaceResized(const u32 width, const u32 height) {
    }

    /**
     * フラグメント復旧を行う
     */
    virtual void onResume() {
    }

    /**
     * 毎フレーム更新を行う
     */
    virtual void onUpdate() {
    }

    /**
     * フラグメント休止を行う
     */
    virtual void onPause() {
    }

    /**
     * 解放処理を行う
     */
    virtual void onDestroy() {
    }

    virtual void onDetatch() {
        this->controller = NULL;
    }

    /**
     * アタッチされているアプリを取得する
     */
    virtual JointApplicationBase* getApplication() const;

    /**
     * フラグメント用コントローラーを取得する
     */
    virtual ApplicationFragmentController* getController() const {
        return controller;
    }

    /**
     * レンダリングデータを取得する
     */
    virtual MRenderingContext getRenderingContext() const;

    virtual MDevice getWindowDevice() const;

    virtual s32 getId() const {
        return id;
    }

    virtual String getTag() const {
        return tag;
    }

    /**
     *
     */
    template<typename T>
    T* getApplicationTo() const {
        return dynamic_cast<T*>(getApplication());
    }

};

typedef jc_sp<IApplicationFragment> MApplicationFragment;

typedef jc_selp<IApplicationFragment> SApplicationFragment;

}
}

#endif /* IAPPLICATIONFRAGMENT_HPP_ */
