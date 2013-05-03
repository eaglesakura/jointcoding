#include "EGLContextManager.h"

namespace ios {
    EGLContextManager::EGLContextManager() {
        this->binded = jcfalse;
        // make context
        opContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        assert(opContext);
        
        bind();
        {
            state.reset(new jc::gl::GLState());
            vram.reset(new _VRAM());
        }
        unbind();
    }
    
    EGLContextManager::~EGLContextManager() {
        this->dispose();
    }
    
    /**
     * コンテキストを関連づいたステート状態を取得する。
     */
    MGLState EGLContextManager::getState() {
        assert(state);
        return state;
    }
    
    /**
     * vramを取得する。
     */
    VRAM EGLContextManager::getVRAM() {
        assert(vram);
        return vram;
    }
    
    /**
     * 握っている資源を明示的に開放する
     */
    void EGLContextManager::dispose() {
        state.reset();
        vram.reset();
        
        if(opContext) {
            opContext = nil;
        }
    }
    
    /**
     * EGLをアプリが専有する
     */
    void EGLContextManager::bind() {
        if(!binded) {
            binded = [EAGLContext setCurrentContext:opContext];
            assert(binded);
        }
    }
    
    /**
     * EGLの占有を解除する
     */
    void EGLContextManager::unbind() {
        if(binded) {
            binded = [EAGLContext setCurrentContext:nil];
            assert(binded);
            binded = jcfalse;
        }
    }
    
}