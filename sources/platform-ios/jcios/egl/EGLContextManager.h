#ifndef com_eaglesakura_ios_sample_EGLContextManager_h
#define com_eaglesakura_ios_sample_EGLContextManager_h

#import "jointcoding-ios.h"
#import <OpenGLES/EAGL.h>

namespace ios {
    using namespace jc;
    using namespace jc::gl;

    /**
     * EGLContextProtocolのiOS向け実装
     */
    class EGLContextManager : public EGLContextProtocol {

        // state cache
        MGLState state;

        // vram cache
        VRAM vram;

        // バインド済みの場合true
        jcboolean binded;
    public:
        EGLContextManager();

        virtual ~EGLContextManager();

        /**
         * コンテキストを関連づいたステート状態を取得する。
         */
        virtual MGLState getState();

        /**
         * vramを取得する。
         */
        virtual VRAM getVRAM();

        /**
         * 握っている資源を明示的に開放する
         */
        virtual void dispose();

        /**
         * EGLをアプリが専有する
         */
        virtual void bind();

        /**
         * EGLの占有を解除する
         */
        virtual void unbind();
    public: /* objective-c */
        
        // EGLContext for iOS
        EAGLContext *opContext;
    };
}


#endif
