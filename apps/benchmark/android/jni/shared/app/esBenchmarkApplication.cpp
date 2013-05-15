/*
 * esBenchmarkApplication.cpp
 *
 *  Created on: 2013/05/06
 */

#include    "esBenchmarkApplication.h"
#include    "jcandroid/egl/sdk/SdkDeviceManagerContext.hpp"

namespace es {

BenchmarkApplication::BenchmarkApplication() {
}

BenchmarkApplication::~BenchmarkApplication() {
    jclogf("delete 0x%x", this);
}

/**
 * サーフェイスの初期化に必要なスペックのリクエストを受け取る。
 * 可能な限りリクエストに沿ったサーフェイスを作成する。
 */
SurfaceSpecs BenchmarkApplication::getRenderingSurfaceSpecs() const {
    SurfaceSpecs specs;
//    specs.extensions.enable(SurfaceSupecExtension_AndroidTextureView);
    return specs;
}

/**
 * アプリ初期化を行わせる
 * メソッド呼び出し時点でデバイスロック済み。
 */
void BenchmarkApplication::onAppInitialize() {
    jclogf("onAppInitialize 0x%x", this);

    getDevice()->getState()->blendEnable(jctrue);
    getDevice()->getState()->blendFunc(GLBlendType_Alpha);
}

void BenchmarkApplication::loadTexture(MDevice subDevice) {
    try {
        jclog("lock start");
        DeviceLock lock(subDevice, jctrue);

        Thread::sleep(1000);
        jclog("load start");
        texture = TextureImage::decode(subDevice, Uri::fromAssets("images/test.png"), PixelFormat_RGBA8888);

        Thread::sleep(500);
        jclog("load finish");
    } catch (Exception &e) {
        jcloge(e);
    }
}

/**
 * アプリのサーフェイスリサイズを行わせる
 * メソッド呼び出し時点でデバイスロック済み。
 */
void BenchmarkApplication::onAppSurfaceResized(const s32 width, const s32 height) {
    jclogf("onAppSurfaceResized 0x%x", this);
}

/**
 * メインループを行わせる
 * メソッド呼び出し時点でデバイスロック済み
 */
void BenchmarkApplication::onAppMainUpdate() {
}

/**
 * メインレンダリングを行わせる。
 * メソッド呼び出し時点でデバイスロック済み。
 * フロントバッファ転送もメソッド内で行わせる。
 */
void BenchmarkApplication::onAppMainRendering() {
    MGLState state = getDevice()->getState();
    {
//        state->clearColorf(0, 0, (float)(rand() % 0xFF) / 255.0f, 0);
        state->clearColorf(0, 1, 1, 1);
        state->clearSurface();
    }

    {
        MSpriteManager spriteManager = getSpriteManager();
        spriteManager->renderingArea(createRectFromXYWH<float>(1, 1, 512, 512), 0xFFFF00FF, 0x0000FFFF);

        if (texture) {
            texture->resetDevice(device);
            spriteManager->renderingImage(texture, 128, 128, Color::fromRGBAf(1, 1, 1, 0.75f));

            static float rotate = 0;
            spriteManager->renderingImage(texture, createRectFromXYWH<float>(256, 256, texture->getWidth(), texture->getHeight()), rotate += 1, Color::fromRGBAf(1, 1, 1, 0.5f));
        }
    }

    device->postFrontBuffer();
}

/**
 * アプリ休止処理
 * 音声の休止等の処理を行う
 */
void BenchmarkApplication::onAppPause() {
    jclogf("onAppPause 0x%x", this);
}

/**
 * アプリの再開処理
 * 音声の再開等の処理を行う
 */
void BenchmarkApplication::onAppResume() {
    jclogf("onAppResume 0x%x", this);
}

/**
 * アプリのメモリ解放を行う
 * メソッド呼び出し時点でデバイスロック済み。
 */
void BenchmarkApplication::onAppDestroy() {
    jclogf("destroy %x", this);
}

}

