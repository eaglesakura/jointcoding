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

@interface JointApplicationViewController : UIViewController<ES20Delegate> {
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
 * アプリの実行が有効状態ならYES
 */
- (BOOL) validApplication;

/**
 * JointApplicationの生成を行う
 */
- (void) createApplicationContext;

@end
