//
//  JointApplicationViewController.m
//  com.eaglesakura.ios.sample
//
//  Created by 山下 武志 on 2013/05/06.
//
//

#import "JointApplicationViewController.h"

// FIXME 後で直す
#include "benchmark.hpp"

@interface JointApplicationViewController ()
/**
 * メインループ処理
 */
- (void) appMainLoop:(id) option;

@end

@implementation JointApplicationViewController

@synthesize surface = _surface;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    assert(self);
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // アプリコンテキストを生成する
    [self createApplicationContext];

    // フルスクリーンでViewを作成
    self->_surface = [[JCOpenGLES20View alloc]initWithFrame:[[UIScreen mainScreen] bounds]];
    [self setView:_surface];
    _surface.delegate = self;
    
}

// 解放処理を行う
- (void) viewDidDisappear:(BOOL)animated {
    if(application) {
        ApplicationQueryKey key;
        key.main_key = JointApplicationProtocol::PostKey_StateRequest;
        application->postParams(&key, &JointApplicationProtocol::State_Destroyed, 1);
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
        // アプリにデバイスを渡す
        application->dispatchSurfaceCreated(_surface.device);
        
        // メインスレッドを開始する
        [self performSelectorInBackground:@selector(appMainLoop:) withObject:self];

        // アプリを開始する
        {
            ApplicationQueryKey key;
            key.main_key = JointApplicationProtocol::PostKey_StateRequest;
            application->postParams(&key, &JointApplicationProtocol::State_Running, 1);
        }
    }
}

/**
 * サーフェイスサイズが変更された
 */
-(void) shouldSurfaceResize:(id)es20view {
    jclog("surface resizestart");
    
    MDevice device = _surface.device;
    assert(device);
}

/**
 * リサイズが行われた
 */
- (void) didSurfaceResized:(id) es20view {
    jclog("surface resized");
    
    MDevice device = _surface.device;
    assert(device);
    
    // 一時的にデバイスのロックを行えなくする
    device->addFlag(DeviceFlag_NoLock);
    MutexLock lock(device->getGPUMutex());
    
    // リサイズ命令を送信
    const Vector2i surfaceSize = device->getSurfaceArea().wh();
    ApplicationQueryKey key;
    key.main_key = JointApplicationProtocol::PostKey_SurfaceSize;
    application->postParams(&key, (const s32*)(&surfaceSize), 2);

    // デバイスの利用を再開させる
    device->removeFlag(DeviceFlag_NoLock);
}

- (BOOL) validApplication {
    if(!application) {
        return NO;
    }
    
    return  YES;
}

- (void) createApplicationContext {
    self->application.reset(new es::BenchmarkApplication());
}


- (void) appMainLoop:(id)option {
    application->dispatchMainLoop();
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
