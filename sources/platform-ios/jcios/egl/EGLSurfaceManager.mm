//
//  EGLSurfaceManager.mm
//  com.eaglesakura.ios.sample
//
//

#include "EGLSurfaceManager.h"

namespace ios {
    
    EGLSurfaceManager::EGLSurfaceManager() {
        width = height = 0;
        
        buffers.color = buffers.depth_stencil = buffers.frame = 0;
    }
    
    EGLSurfaceManager::~EGLSurfaceManager() {
        this->dispose();
    }
    
    
    /**
     * レンダリングサーフェイスの大きさを変更する
     */
    void EGLSurfaceManager::onResized(const Vector2f newSize) {
        width = newSize.x;
        height = newSize.y;
    }
    
    
    /**
     * EGLに関連付ける
     */
    void EGLSurfaceManager::bind() {
        state->bindFrameBuffer(GL_FRAMEBUFFER, buffers.frame);
    }
    
    /**
     * EGLから関連付けを解除する
     */
    void EGLSurfaceManager::unbind() {
        state->bindFrameBuffer(GL_FRAMEBUFFER, 0);
    }
    
    
    /**
     * 握っている資源を明示的に開放する
     */
    void EGLSurfaceManager::dispose() {
        if(!buffers.frame) {
            // 既に解放済みなら何もしない
            return;
        }
        
        
        // 既存の関連付けを解除
        bind();
        {
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            assert_gl();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            assert_gl();
        }
        unbind();
        
        // バッファ削除
        {
            // フレームバッファ削除
            glDeleteFramebuffers(1, &buffers.frame);
            buffers.frame = 0;
            assert_gl();
            
            // 色用バッファを削除
            glDeleteRenderbuffers(1, &buffers.color);
            buffers.color = 0;
            assert_gl();
            
            // 深度・ステンシル（型抜き）混合バッファを削除
            glDeleteRenderbuffers(1, &buffers.depth_stencil);
            buffers.depth_stencil = 0;
            assert_gl();
        }
    }
    
    /**
     * インスタンスを作成する
     */
    jc_sp<EGLSurfaceManager> EGLSurfaceManager::createInstance( const Vector2f surfaceSize, jc_sp<EGLContextManager> eglContext ) {
        jc_sp<EGLSurfaceManager> result(new EGLSurfaceManager());
        MGLState state = eglContext->getState();
        result->state = state;
        
        eglContext->bind();
        {
            // バッファの作成
            {
                // フレームバッファ作成
                glGenFramebuffers(1, &result->buffers.frame);
                
                // 色用バッファを作成
                glGenRenderbuffers(1, &result->buffers.color);
                // 深度・ステンシル（型抜き）混合バッファを作成
                glGenRenderbuffers(1, &result->buffers.depth_stencil);
                
                // 作成できていることを確認する
                assert(result->buffers.frame != 0);
                assert(result->buffers.color != 0);
                assert(result->buffers.depth_stencil != 0);
            }
            
            // 描画用に関連付ける
            {
                state->bindFrameBuffer(GL_FRAMEBUFFER, result->buffers.frame);
                // FrameBuffer <--> Color RenderBufferの関連付け
                {
                    glBindRenderbuffer(GL_RENDERBUFFER, result->buffers.color);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, result->buffers.color);
                }
                assert_gl();
                
                // FrameBuffer <--> Depth & Stencil RenderBufferの関連付け
                {
                    glBindRenderbuffer(GL_RENDERBUFFER, result->buffers.depth_stencil);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, result->buffers.depth_stencil);
                    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, result->buffers.depth_stencil);
                }
                assert_gl();
            }
        }
        eglContext->unbind();
        
        return result;
    }
    
}