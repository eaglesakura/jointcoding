/*
 * NDKPlatformContext.h
 *
 *  Created on: 2013/05/16
 */

#include    "jointcoding-android.h"
#include    "jc/framework/JointApplicationFramework.h"
#include    "android-classes/ndkJointApplicationRenderer.h"
#include    "android-classes/ndkWindowDeviceManager.h"
#include    "jcandroid/framework/NDKDataBank.hpp"

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

    jc_sp<NDKDataBank> dataBank;
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

    /**
     * プラットフォームへ明示的に値を送信する。
     * プラットフォームで書き込まれた値はparamsへ反映される
     *
     * ハンドリングが行われなければfalseを返す
     */
    virtual jcboolean postParams(const s32 main_key, const s32 sub_key, unsafe_array<String> *params);

    /**
     * post/receive出来ないネイティブクラス等のIOに利用するデータバンク
     * add/popのみが行える。適度にキャストして利用する。
     */
    virtual MDataBank getDataBank() {
        return dataBank;
    }
};

}
