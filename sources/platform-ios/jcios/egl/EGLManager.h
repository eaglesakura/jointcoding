#ifndef com_eaglesakura_ios_sample_EGLManager_h
#define com_eaglesakura_ios_sample_EGLManager_h

#import "jointcoding-ios.h"
#import "EGLContextManager.h"

namespace ios {
    using namespace jc;
    using namespace jc::gl;
    
    /**
     * EGL管理のiOS版
     */
    class EGLManager : public EGLProtocol {
        /**
         * アタッチされているスレッドID
         */
        MThreadID threadId;
        
        /**
         * 最後にバインドしたコンテキスト
         */
        jc_sp<EGLContextManager>    eglContext;
    public:
        EGLManager();
        virtual ~EGLManager();
        
        /**
         * ステータスを取得する
         */
        virtual EGLStatus_e getStatus() const;
        
        /**
         * 指定したコンテキストとサーフェイスをカレントスレッドに割り当てる。
         */
        virtual void current(jc_sp<EGLContextProtocol> context, jc_sp<EGLSurfaceProtocol> surface);
        
        /**
         * 描画用のディスプレイにバックバッファの内容を転送する。
         * 転送方法については各デバイスの実装に従う。
         * このメソッドは完了するまでブロックされることを保証しない（デバイス依存、基本的にブロッキングになるように調整している）
         */
        virtual jcboolean postFrontBuffer(jc_sp<EGLSurfaceProtocol> displaySurface);
        
        /**
         * 握っているEGL資源を明示的に開放する。
         */
        virtual void dispose();
        
    public: /* objective-c */
        /**
         * iOS版のView
         */
        __weak id uiView;
    };
}

#endif
