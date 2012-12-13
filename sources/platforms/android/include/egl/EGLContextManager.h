/*
 * ndkEGLContextManager.h
 *
 *  Created on: 2012/07/21

 */

#ifndef NDKEGLCONTEXTMANAGER_H_
#define NDKEGLCONTEXTMANAGER_H_

#include "EGLImpl.h"

namespace ndk {
using namespace jc;
using namespace jc::gl;

/**
 * EGLのコンテキスト操作を行うクラス。
 */
class EGLContextManager: public jc::Object, public EGLContextProtocol {
    /**
     * 関連づいたディスプレイ
     */
    EGLDisplay display;

    /**
     * 作成するコンテキスト用のコンフィグデータ
     */
    EGLConfig config;

    /**
     *
     */
    EGLContext context;

    /**
     * 状態管理
     */
    MGLState state;

    /**
     * VRAM管理
     */
    VRAM vram;

public:
    EGLContextManager(EGLContext context, EGLDisplay display, EGLConfig config);
    virtual ~EGLContextManager();

    virtual EGLContext getContext() {
        return context;
    }

    virtual EGLDisplay getDisplay() {
        return display;
    }

    /**
     * コンテキストを関連づいたステート状態を取得する。
     */
    virtual MGLState getState() {
        return state;
    }

    /**
     * VRAMを取得する。
     */
    virtual VRAM getVRAM() {
        return vram;
    }

    /**
     * 資源の開放を行う
     */
    virtual void dispose();
};

/**
 * managed
 */
typedef jc_sp<EGLContextManager> MEGLContextManager;}

#endif /* NDKEGLCONTEXTMANAGER_H_ */
