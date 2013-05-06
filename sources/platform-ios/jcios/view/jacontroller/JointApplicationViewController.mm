//
//  JointApplicationViewController.m
//  com.eaglesakura.ios.sample
//
//  Created by 山下 武志 on 2013/05/06.
//
//

#import "JointApplicationViewController.h"

@interface JointApplicationViewController ()
/**
 * メインループ処理
 */
- (void) appMainLoop:(id) option;

@end

@implementation JointApplicationViewController

@synthesize surface = _surface;
@synthesize state = _state;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    assert(self);
    self->_state = JointApplicationState_Null;
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // フルスクリーンでViewを作成
    self->_surface = [[JCOpenGLES20View alloc]initWithFrame:[[UIScreen mainScreen] bounds]];
    [self setView:_surface];
    _surface.delegate = self;
    
    // メインスレッドを開始する
    [self performSelectorInBackground:@selector(appMainLoop:) withObject:self];
}

// 解放処理を行う
- (void) viewDidDisappear:(BOOL)animated {
    _state = JointApplicationState_Destroyed;
    @synchronized(self) {
        if(_surface ) {
            [_surface disposeGL];
        }
    }
    [super viewDidDisappear:animated];
}


/**
 * 初期化完了した
 */
- (void) didInitializeComplete:(id) es20view {
    jclog("surface initialized");
    
    // アプリ初期化を行わせる
    @synchronized(self) {
        [self createApplicationContext];
    }
}

-(void) shouldSurfaceResize:(id)es20view {
    jclog("surface resizestart");
    _state = JointApplicationState_Resizing;
}

/**
 * リサイズが行われた
 */
- (void) didSurfaceResized:(id) es20view {
    jclog("surface resized");
    
    // リサイズ命令を送信
    @synchronized(self){
        MDevice device = _surface.device;
        Vector2i surfaceSize = device->getSurfaceArea().wh();
        application->dispatchSurfaceResized(surfaceSize.x, surfaceSize.y);
    }
    _state = JointApplicationState_Running;
}

- (BOOL) validApplication {
    if(_state != JointApplicationState_Running) {
        return  NO;
    }
    
    if(!application) {
        return NO;
    }
    
    return  YES;
}

- (void) createApplicationContext {
    assert(false);
}


- (void) appMainLoop:(id)option {
    while(_state != JointApplicationState_Destroyed) {
        @synchronized(self) {
            jcboolean sleep = jcfalse;
            if( [self validApplication] ) {
                // アプリが有効ならフレーム処理を行う
                application->dispatchMainLoop();
            } else {
                // スリープを行わせる
                sleep = jctrue;
            }
        }
        
        if(sleep) {
            Thread::sleep(10);
        }
    }
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
