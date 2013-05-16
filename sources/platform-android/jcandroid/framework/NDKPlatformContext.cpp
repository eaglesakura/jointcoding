/*
 * NDKPlatformContext.cpp
 *
 *  Created on: 2013/05/16
 */

#include    "jcandroid/framework/NDKPlatformContext.h"

namespace ndk {

using namespace jc;
using namespace jc::gl;

NDKPlatformContext::NDKPlatformContext(jc_sp<JointApplicationRenderer> renderer) : PlatformContext(Platform_Android) {
    this->renderer = renderer;
}

NDKPlatformContext::~NDKPlatformContext() {

}

/**
 * 制御用のスレイブデバイスを作成する
 */
MDevice NDKPlatformContext::createSlaveDevice() {
    return MDevice();
}

/**
 * 新規タスクを生成する
 */
void NDKPlatformContext::startNewtask(const s32 uniqueId, const s32 user_data) {

}

}

