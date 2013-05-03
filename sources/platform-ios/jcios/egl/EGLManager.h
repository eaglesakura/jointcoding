#ifndef com_eaglesakura_ios_sample_EGLManager_h
#define com_eaglesakura_ios_sample_EGLManager_h

#import "jointcoding-ios.h"


namespace ios {
    using namespace jc;
    using namespace jc::gl;
    
    /**
     * EGL管理のiOS版
     */
    class EGLManager : public EGLProtocol {
        /**
         * iOS版のView
         */
        jc_weak_id   uiView;
        
        /**
         * アタッチされているスレッドID
         */
        MThreadID threadId;

    public:
        EGLManager(jc_weak_id uiView);
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
    };
}

#endif
