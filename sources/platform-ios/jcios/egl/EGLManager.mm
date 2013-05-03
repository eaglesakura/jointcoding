//
//  EGLManager.mm
//  com.eaglesakura.ios.sample
//
//

#include "EGLManager.h"
#import "EGLContextManager.h"
#improt "EGLSurfaceManager.h"

namespace ios {
    EGLManager::EGLManager(jc_weak_id uiView) {
        this->uiView = uiView;
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
            
            
        } else {
            // contextとsurface両方がNULLでなければならない。
            assert(!context && !surface);
        }
    }
    
    /**
     * 描画用のディスプレイにバックバッファの内容を転送する。
     * 転送方法については各デバイスの実装に従う。
     * このメソッドは完了するまでブロックされることを保証しない（デバイス依存、基本的にブロッキングになるように調整している）
     */
    jcboolean EGLManager::postFrontBuffer(jc_sp<EGLSurfaceProtocol> displaySurface) {
        assert(false);
        return jcfalse;
    }
    
    /**
     * 握っているEGL資源を明示的に開放する。
     */
    void EGLManager::dispose() {
        if(uiView) {
            uiView.object = nil;
        }
    }
}