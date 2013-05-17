/*
 * NDKPlatformContext.h
 *
 *  Created on: 2013/05/16
 */

#include    "jointcoding-android.h"
#include    "jc/framework/JointApplicationFramework.h"
#include    "android-classes/ndkJointApplicationRenderer.h"
#include    "android-classes/ndkWindowDeviceManager.h"

namespace ndk {

using namespace jc;
using namespace jc::gl;

class NDKPlatformContext: public PlatformContext {
    /**
     * SDK側のレンダラ
     */
    jc_sp<JointApplicationRenderer> renderer;

    /**
     * SDK側のウィンドウクラス
     */
    jc_sp<WindowDeviceManager> windowDeviceManager;
public:
    NDKPlatformContext(jc_sp<JointApplicationRenderer> renderer);

    virtual ~NDKPlatformContext();

    /**
     * 制御用のスレイブデバイスを作成する
     */
    virtual MDevice createSlaveDevice() const;

    /**
     * 新規タスクを生成する
     * タスクはJointApplicationBase::dispatchNewtask()をコールし、その中で処理を行う
     */
    virtual void startNewtask( const s32 uniqueId, const s32 user_data);
};

}
