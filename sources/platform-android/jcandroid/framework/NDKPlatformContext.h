/*
 * NDKPlatformContext.h
 *
 *  Created on: 2013/05/16
 */

#include    "jointcoding-android.h"
#include    "jc/framework/JointApplicationFramework.h"
#include    "android-classes/ndkJointApplicationRenderer.h"

namespace ndk {

using namespace jc;
using namespace jc::gl;

class NDKPlatformContext: public PlatformContext {
    /**
     * SDK側のレンダラ
     */
    jc_sp<JointApplicationRenderer> renderer;

public:
    NDKPlatformContext(jc_sp<JointApplicationRenderer> renderer);

    virtual ~NDKPlatformContext();

    /**
     * 制御用のスレイブデバイスを作成する
     */
    MDevice createSlaveDevice();

    /**
     * 新規タスクを生成する
     */
    void startNewtask( const s32 uniqueId, const s32 user_data);
};

}
