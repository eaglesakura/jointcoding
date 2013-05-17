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

}

