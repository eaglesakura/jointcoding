//
//  JointApplicationViewController.h
//  com.eaglesakura.ios.sample
//
//  Created by 山下 武志 on 2013/05/06.
//
//

#import <UIKit/UIKit.h>
#import "JCOpenGLES20View.h"
#include "jointcoding-ios.h"
#include "jc/framework/app/JointApplicationBase.h"

enum JointApplicationState_e {
    /**
     * 初期化中
     */
    JointApplicationState_Null,
    
    /**
     * 実行中
     */
    JointApplicationState_Running,
    
    /**
     * リサイズ中
     */
    JointApplicationState_Resizing,
    
    /**
     * 廃棄済み
     */
    JointApplicationState_Destroyed,
};

@interface JointApplicationViewController : UIViewController<ES20Delegate> {
    /**
     * 現在の状態
     */
    JointApplicationState_e _state;
    
    /**
     * レンダリングサーフェイス
     */
    JCOpenGLES20View    *_surface;
    
    /**
     * 共有アプリ
     */
    jc_sp<JointApplicationBase>    application;
}

/**
 * レンダリングサーフェイス
 */
@property(readonly) JCOpenGLES20View*   surface;

/**
 * 現在の実行状態
 */
@property(readonly) JointApplicationState_e state;

/**
 * アプリの実行が有効状態ならYES
 */
- (BOOL) validApplication;

/**
 * JointApplicationの生成を行う
 */
- (void) createApplicationContext;

@end
