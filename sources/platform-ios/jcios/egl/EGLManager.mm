//
//  EGLManager.mm
//  com.eaglesakura.ios.sample
//
//

#include "EGLManager.h"
#import "EGLContextManager.h"
#import "EGLSurfaceManager.h"
#import <OpenGLES/EAGLDrawable.h>

using namespace ios;
using namespace jc;
using namespace jc::gl;

namespace ios {
    EGLManager::EGLManager() {
        this->uiView = nil;
    }
    
    EGLManager::~EGLManager() {
        this->dispose();
    }
    
    /**
     * ステータスを取得する
     */
    EGLStatus_e EGLManager::getStatus() const {
        // スレッドIDを得ていなければ、どのスレッドにも属していない
        if (!threadId) {
            return EGLStatus_Ready;
        }
        
        // 現在のスレッドIDを取得
        ThreadID current_id;
        
        // 同一スレッドだったらAttached、それ以外のスレッドだったらBusy
        return current_id.equals(threadId) ? EGLStatus_Attached : EGLStatus_Busy;
    }
    
    /**
     * 指定したコンテキストとサーフェイスをカレントスレッドに割り当てる。
     */
    void EGLManager::current(jc_sp<EGLContextProtocol> context, jc_sp<EGLSurfaceProtocol> surface) {
        if(context && surface) {
            // コンテキストを現在のスレッドへバインドする
            jc_sp<ios::EGLContextManager>    eglContext = jc::downcast<ios::EGLContextManager>(context);
            jc_sp<ios::EGLSurfaceManager>    eglSurface = downcast<ios::EGLSurfaceManager>(surface);
            
            assert(eglContext);
            assert(eglSurface);
            
            eglContext->bind();
            eglSurface->bind();
            
            assert_gl();
            
            // 最後にバインドしたコンテキストを覚えておく
            this->eglContext = eglContext;
            
            // 現在のスレッドを登録する
            threadId.reset(new ThreadID());
        } else {
            // contextとsurface両方がNULLでなければならない。
            assert(!context && !surface);
            
            if(eglContext) {
                // バインドを解除する
                eglContext->unbind();
                eglContext.reset();
                
                threadId.reset();
            }
        }
    }
    
    /**
     * 描画用のディスプレイにバックバッファの内容を転送する。
     * 転送方法については各デバイスの実装に従う。
     * このメソッドは完了するまでブロックされることを保証しない（デバイス依存、基本的にブロッキングになるように調整している）
     */
    jcboolean EGLManager::postFrontBuffer(jc_sp<EGLSurfaceProtocol> displaySurface) {
        // コンテキストがバインド済みでなければならない
        assert(eglContext);
        assert(displaySurface);
        
        // ディスプレイにバインドしなおす
        jc_sp<EGLSurfaceManager> eglSurface = downcast<EGLSurfaceManager>(displaySurface);
        assert(eglSurface);
        eglSurface->bind();
        
        // EAGLContextの機能で転送する
        [eglContext->opContext presentRenderbuffer:GL_RENDERBUFFER];
        assert_gl();
        return jctrue;
    }
    
    /**
     * 握っているEGL資源を明示的に開放する。
     */
    void EGLManager::dispose() {
        uiView = nil;
    }
}