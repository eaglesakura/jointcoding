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

-(void) testRender:(id) _temp;
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
        if(![self isInitializedGL]) {
            [self initializeGL];
        }
        
        // resized
        [self resizeSurface];
    }
    
    
    [self testRender:nil];
//    [self performSelectorInBackground:@selector(testRender:) withObject:nil];
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
    
    _device.reset( new Device() );
    _device->setEGL(eglManager);
    _device->setContext(eglContext);
    _device->setSurface(eglSurface);
}

-(void) resizeSurface {
    assert(eglSurface);

    try {
        DeviceLock  lock(_device, jctrue);
        {
            // ViewとEAGLContextをバインドする
            {
                const BOOL completed = [eglContext->opContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)self.layer];
                assert_gl();
                assert(completed);
            }
            // リサイズ通知を出して、残りのバッファを更新させる
            eglSurface->onResized();
            
            // コンテキストを同期する
            eglContext->getState()->syncContext();
        }
    }catch( EGLException &e) {
        jcloge(e);
        assert(false);
    }
    
}

/**
 * GL初期化済みであればYESを返す
 */
- (BOOL) isInitializedGL {
    @synchronized(self) {
        return _device ? YES : NO;
    }
}


- (void) disposeGL {
    
    @synchronized(self) {
        if(!_device) {
            return;
        }
        
        jclog("dispose GL");
        _device->addFlags(DeviceFlag_RequestDestroy);
        
        _device.reset();
        eglSurface.reset();
        eglManager.reset();
        eglContext.reset();
   
    }
}

- (void) testRender:(id)_temp {
    Thread::sleep(10);
    jc::gl::MGLState state = eglContext->getState();
//    while(true)
    try {
        DeviceLock lock(_device, jctrue);
        {
            MSpriteManager spriteManager = SpriteManager::createInstance(_device);

//            state->clearColorf(0, (float)(rand() & 0xFF) / 255.0f, 1, 1);
            state->clearColorf(0.0f, 1.0f, 1.0f, 1.0f);
            state->clear();
            
            spriteManager->renderingRect(0, 0, 512, 512, 0xFF0000FF);
            
            eglManager->postFrontBuffer(eglSurface);
        }
    }catch(EGLException &e) {
        jcloge(e);
    }
}

@end
