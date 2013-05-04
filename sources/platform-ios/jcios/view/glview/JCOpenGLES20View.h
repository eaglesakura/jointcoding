#import <UIKit/UIKit.h>
#import "jointcoding-ios.h"
#import "EGLContextManager.h"
#import "EGLSurfaceManager.h"
#import "EGLManager.h"

@interface JCOpenGLES20View : UIView
{
    // レンダリングコンテキスト
    jc_sp<ios::EGLManager>           eglManager;
    jc_sp<ios::EGLContextManager>    eglContext;
    jc_sp<ios::EGLSurfaceManager>    eglSurface;

    // レンダリングデバイス
    jc::gl::MDevice _device;
}

/**
 * レンダリングデバイスへのアクセサ
 */
@property (readonly) jc::gl::MDevice    device;

/**
 * GL初期化済みであればYESを返す
 */
- (BOOL) isInitializedGL;

@end
