/*
 * NDKPlatformContext.cpp
 *
 *  Created on: 2013/05/16
 */

#include    "jcandroid/framework/NDKPlatformContext.h"
#include    "android-classes/ndkDeviceManager.h"
#include    "jcandroid/egl/sdk/SdkDeviceManagerContext.hpp"

namespace ndk {

using namespace jc;
using namespace jc::gl;

NDKPlatformContext::NDKPlatformContext(jc_sp<JointApplicationRenderer> renderer) : PlatformContext(Platform_Android) {
    this->renderer = renderer;
    assert(renderer);

    this->windowDeviceManager = WindowDeviceManager::global(renderer->getWindowDevice_unsafe());
    assert(windowDeviceManager);

    {
        // レンダリングデバイスを得る
        const MDevice device = SdkDeviceManagerContext::getDeviceFromSdkDeviceManager(windowDeviceManager->getObject());
        assert(device);

        // Native利用のためのレンダラーを取得する
        this->windowDevice = device;
    }

}

NDKPlatformContext::~NDKPlatformContext() {
    windowDeviceManager->dispose();
    windowDeviceManager.reset();

    windowDevice.reset();

}

/**
 * 制御用のスレイブデバイスを作成する
 */
MDevice NDKPlatformContext::createSlaveDevice() const {
    // スレイブデバイスを生成する
    jobject jDeviceManager = windowDeviceManager->createSlaveDevice_unsafe();

    // 中身からNDK用のデバイスを抜き出す
    const MDevice device = SdkDeviceManagerContext::getDeviceFromSdkDeviceManager(jDeviceManager);
    assert(device);

    // SDK側を解放する
    DeviceManager::dispose_(jDeviceManager);

    // 参照の廃棄を行う
    {
        CALL_JNIENV();
        env->DeleteLocalRef(jDeviceManager);
    }

    // 残ったデバイスを返す
    return device;
}

/**
 * 新規タスクを生成する
 */
void NDKPlatformContext::startNewtask(const s32 uniqueId, const s32 user_data) {
    // タスクの生成はSDK側に任せる
    renderer->startNewtask(uniqueId, user_data);
}
/**
 * プラットフォームへ明示的に値を送信する。
 * プラットフォームで書き込まれた値はparamsへ反映される
 *
 * ハンドリングが行われなければfalseを返す
 */
jcboolean NDKPlatformContext::postParams(const s32 main_key, const s32 sub_key, unsafe_array<String> *params) {
    assert(params);
    jobjectArray array = JointApplicationRenderer::newStringArray_unsafe(params->length);
    assert(array);

    // 送信用パラメータを生成
    c2stringArray(array, params->ptr, params->length);

    const jcboolean result = renderer->receiveParams(main_key, sub_key, array);

    // 生成された値を受け取る
    if (result) {
        j2stringArray(array, params);
    }

    // パラメータを削除
    CALL_JNIENV();
    env->DeleteLocalRef(array);

    return result;
}

}

