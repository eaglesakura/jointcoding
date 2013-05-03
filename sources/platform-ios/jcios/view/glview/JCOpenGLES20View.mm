#import "JCOpenGLES20View.h"
#import <QuartzCore/CAEAGLLayer.h>

using namespace ios;

@interface JCOpenGLES20View()
/**
 * GL ES 2.0の初期化を行う
 */
-(void) initializeGL;

/**
 * GL ES 2.0の解放処理を行う
 */
-(void) disposeGL;

/**
 * FrameBufferのリサイズを行う
 */
-(void) resizeSurface;
@end

@implementation JCOpenGLES20View

@synthesize device = _device;


+ (Class) layerClass{
    return [CAEAGLLayer class];
}

/**
 * EGLの初期化を行う。
 */
- (void) initLayer {
    CAEAGLLayer *pLayer = ( CAEAGLLayer*)self.layer;
    pLayer.opaque = TRUE;
    pLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                 [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                 kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat, nil];
}

- (id)init {
    jclog("[-(id)init] not support!!");
    // not support!!
    assert(false);
    return self;
}

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    assert(self);
    [self initLayer];
    self.backgroundColor = [UIColor whiteColor];
    return self;
}

- (void) layoutSubviews {
    [super layoutSubviews];
    
    @synchronized(self) {
        // GL initialize
        [self initializeGL];
    }
}

- (void) dealloc {
    [self disposeGL];
}

// GLの初期化を行う
- (void) initializeGL {
    // スケールファクタを設定する
    // Retinaディスプレイの場合でも正しい解像度でレンダリングバッファを作成するようになる。
    self.contentScaleFactor = [UIScreen mainScreen].scale;
    
    eglContext.reset(new EGLContextManager());
    eglSurface = EGLSurfaceManager::createInstance(eglContext);
    eglManager.reset( new EGLManager());
    
    // サーフェイスのリサイズを行う
    [self resizeSurface];

//    eglManager->current(eglContext, eglSurface);
//    {
//        jc::gl::MGLState state = eglContext->getState();
//        state->clearColorf(0, 1, 1, 1);
//        state->clearSurface();
//        assert_gl();
//        
//        eglManager->postFrontBuffer(eglSurface);
//    }
//    eglManager->current(EGL_NULL_CONTEXT, EGL_NULL_SURFACE);
}

-(void) resizeSurface {
    assert(eglSurface);
    
    eglContext->bind();
    eglSurface->bind();
    {   
        const float scaleFactor = [self contentScaleFactor];
        const int rendering_buffer_width = (int)(self.bounds.size.width * scaleFactor);
        const int rendering_buffer_height = (int)(self.bounds.size.height * scaleFactor);
        jclogf("Rendering Request Size(%d x %d) x%.1f", rendering_buffer_width, rendering_buffer_height, scaleFactor);
    }
    {
        const BOOL completed = [eglContext->opContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
        assert_gl();
        assert(completed);
    }
    // リサイズ通知を出して、残りのバッファを更新させる
    eglSurface->onResized();
}

- (void) disposeGL {
    eglSurface.reset();
    eglManager.reset();
    eglContext.reset();
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
