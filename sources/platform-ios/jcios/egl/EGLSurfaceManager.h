#ifndef com_eaglesakura_ios_sample_EGLSurfaceManager_h
#define com_eaglesakura_ios_sample_EGLSurfaceManager_h


#include "jointcoding-ios.h"
#include "EGLContextManager.h"

namespace ios {
    using namespace jc;
    using namespace jc::gl;
    
    /**
     * 描画用サーフェイスを設定する
     */
    class   EGLSurfaceManager : public EGLSurfaceProtocol {
        /**
         * コンテキスト保持クラス
         */
        MGLState state;
        
        /**
         * 描画サーフェイスの幅
         */
        s32 width;
        
        /**
         * 描画サーフェイスの高さ
         */
        s32 height;
        
        struct {
            /**
             * レンダリング用フレームバッファ
             */
            GLuint frame;
            
            /**
             * 色情報バッファ
             */
            GLuint color;
            
            /**
             * 深度・ステンシル混合バッファ
             * depth 24bit
             * stencil 8bit
             */
            GLuint depth_stencil;
        }buffers;
        
    public:
        EGLSurfaceManager();
        
        virtual ~EGLSurfaceManager();
        /**
         * サーフェイスの幅を取得する。
         */
        virtual s32 getWidth() const {
            return width;
        }
        
        /**
         * サーフェイスの高さを取得する。
         */
        virtual s32 getHeight() const {
            return height;
        }
        
        /**
         * 握っている資源を明示的に開放する
         */
        virtual void dispose();
        
        /**
         * レンダリングサーフェイスの大きさを変更する
         */
        virtual void onResized(const Vector2f newSize);
        
        
        /**
         * EGLに関連付ける
         */
        virtual void bind();
        
        /**
         * EGLから関連付けを解除する
         */
        virtual void unbind();
        
        /**
         * インスタンスを作成する
         */
        static jc_sp<EGLSurfaceManager> createInstance( const Vector2f surfaceSize, jc_sp<EGLContextManager> eglContext );
    };
}


#endif
