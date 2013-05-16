/*
 * NDKPlatformContext.h
 *
 *  Created on: 2013/05/16
 */

#include    "jointcoding-android.h"
#include    "jc/framework/JointApplicationFramework.h"
#include    "android-classes/ndkJointApplicationRenderer.h"
<<<<<<< HEAD
#include    "android-classes/ndkWindowDeviceManager.h"
=======
>>>>>>> プラットフォーム制御クラスを追加

namespace ndk {

using namespace jc;
using namespace jc::gl;

class NDKPlatformContext: public PlatformContext {
    /**
     * SDK側のレンダラ
     */
    jc_sp<JointApplicationRenderer> renderer;

<<<<<<< HEAD
    /**
     * SDK側のウィンドウクラス
     */
    jc_sp<WindowDeviceManager> windowDeviceManager;
=======
>>>>>>> プラットフォーム制御クラスを追加
public:
    NDKPlatformContext(jc_sp<JointApplicationRenderer> renderer);

    virtual ~NDKPlatformContext();

    /**
     * 制御用のスレイブデバイスを作成する
     */
<<<<<<< HEAD
    virtual MDevice createSlaveDevice() const;

    /**
     * 新規タスクを生成する
     * タスクはJointApplicationBase::dispatchNewtask()をコールし、その中で処理を行う
     */
    virtual void startNewtask( const s32 uniqueId, const s32 user_data);
=======
    MDevice createSlaveDevice();

    /**
     * 新規タスクを生成する
     */
    void startNewtask( const s32 uniqueId, const s32 user_data);
>>>>>>> プラットフォーム制御クラスを追加
};

}
