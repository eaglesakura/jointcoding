#import <UIKit/UIKit.h>
#import "jointcoding-ios.h"
#import "EGLContextManager.h"
#import "EGLSurfaceManager.h"
#import "EGLManager.h"

@protocol ES20Delegate <NSObject>

@required
/**
 * 初期化完了した
 */
- (void) didInitializeComplete:(id) es20view;

/**
 * リサイズが行われた
 */
- (void) shouldSurfaceResize:(id) es20view;

/**
 * リサイズが行われた
 */
- (void) didSurfaceResized:(id) es20view;

@end


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
 * メッセージング用デリゲートを登録
 */
@property (weak) id<ES20Delegate> delegate;

/**
 * GL初期化済みであればYESを返す
 */
- (BOOL) isInitializedGL;

/**
 * GL ES 2.0の解放処理を行う
 */
-(void) disposeGL;

@end
