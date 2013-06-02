/*
 * esBenchmarkApplication.cpp
 *
 *  Created on: 2013/05/06
 */

#include    "esBenchmarkApplication.h"
#include    "jc/collection/ArrayHandle.hpp"

#include    "jc/framework/assets/FigureAsset.hpp"
#include    "jc/graphics/figure/data/ArchiveFigureDataFactory.h"

#include    "jc/framework/assets/figure/FigureLoader.h"

using namespace jc::fw;
namespace es {

BenchmarkApplication::BenchmarkApplication() {
    rotate = 0;
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

    getWindowDevice()->getState()->blendEnable(jctrue);
    getWindowDevice()->getState()->blendFunc(GLBlendType_Alpha);

    {

        figure.reset(new Figure());

        // フィギュアロード
        jc_sp<ArchiveFigureDataFactory> factory(new ArchiveFigureDataFactory());
        jc_sp<UriTextureFactory> texFactory(new UriTextureFactory());
        texFactory->fromAssets("");
        texFactory->setFilenameExt(".png");

        factory->initialize(Uri::fromAssets("susanow.archive"));

        jc_sp<FigureLoader> loader(new FigureLoader(getWindowDevice(), factory, texFactory));

        // 読み込み対象を指定する
        loader->setLoadTarget(figure);

        // 読込を行う
        loader->load();
    }

    {
        // シェーダー読込
        shader = ShaderProgram::buildFromUri(getWindowDevice(), Uri::fromAssets("basic.vert"), Uri::fromAssets("basic.frag"));
        assert(shader);
    }

    // フィギュアのインスタンスを確保する
    {
        renderer.reset(new BasicFigureRenderer());
        renderer->initialize(getWindowDevice(), shader);

        worldEnv.reset(new EnvironmentInstanceState());

        figure0.reset(renderer->createInstanceState(figure));
        figure0->setEnvironmentState(worldEnv);

        figure1.reset(renderer->createInstanceState(figure));
        figure1->setEnvironmentState(worldEnv);
    }

    // オフスクリーンターゲットを生成
    {
        MDevice device = getWindowDevice();

        const s32 width = 480;
        const s32 height = 640;
        offscreen.reset(new FrameBufferObject(device));
        offscreen->allocColorRenderbuffer(device, PixelFormat_RGBA8888);
        offscreen->allocDepthRenderbuffer(device, 24);

        // オフスクリーンのリサイズを行う
        offscreen->resize(device->getState(), width, height);

        // オフスクリーンテクスチャの確保を行う
        offscreen->allocColorRenderTexture(device, PixelFormat_RGBA8888);
//        offscreen->allocDepthRenderTexture(device);

        offscreen->checkFramebufferStatus();
        offscreen->unbind(device->getState());
    }

//    // テクスチャロードを開始する
    startNewtask(BenchmarkTask_LoadTexture, 0);
}

void BenchmarkApplication::loadTexture(MDevice subDevice) {
    try {
        jclog("lock start");
        DeviceLock lock(subDevice, jctrue);

        Thread::sleep(1000);
        jclog("load start");
        texture = TextureImage::decode(subDevice, Uri::fromAssets("images/test.png"), PixelFormat_RGBA8888);
        texture->unbind();

        Thread::sleep(500);
        subDevice->getVRAM()->gc();

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
    MGLState state = getWindowDevice()->getState();
    {
//        state->clearColorf(0, 0, (float) (rand() % 0xFF) / 255.0f, 0);
        state->clearColorf(0, 1, 1, 1);
        state->clearSurface();
        state->viewport(0, 0, getWindowSize().x, getWindowSize().y);
    }

    {
        state->depthTestEnable(jcfalse);
        MSpriteManager spriteManager = getSpriteManager();
        spriteManager->renderingArea(createRectFromXYWH<float>(1, 1, 512, 512), 0xFFFF00FF, 0x0000FFFF);

        if (texture) {
            spriteManager->renderingImage(texture, 128, 128, Color::fromRGBAf(1, 1, 1, 0.75f));

            static float rotate = 0;
            spriteManager->renderingImage(texture, createRectFromXYWH<float>(256, 256, texture->getWidth(), texture->getHeight()), rotate += 1, Color::fromRGBAf(1, 1, 1, 0.5f));
        }
    }

// rendering 3d

    offscreen->bind(state);
    {
        rotate = jc::normalizeDegree(rotate += 1.0f);

        MDevice device = getWindowDevice();
        MGLState state = device->getState();

        // レンダラーを作成する
        // レンダリングの環境設定
        {
            Vector3f basicLightPos(0, 1, 1);
            Matrix4x4 m;
            m.rotateY(rotate);
            m.rotateX(rotate);
            m.multiply(basicLightPos, &basicLightPos);
            worldEnv->getShadowLight()->setDirection(basicLightPos, Vector3f(0, 0, 0));
        }

        {
            Vector3f camPos(0, 5, 200);
            Matrix4x4 m;
            m.rotateY(rotate);
            m.multiply(camPos, &camPos);
            worldEnv->getMainCamera()->lookAt(camPos, Vector3f(0, -5, 0), Vector3f(0, 1, 0));
            worldEnv->getMainCamera()->projection(50.0f, 500.0f, 45.0f, offscreen->getAspect());
        }
        {
            jc::Transform trans;
            trans.translate.x = 15;
            figure0->setTransform(&trans);
        }
        {
            jc::Transform trans;
            trans.translate.x = -15;
            trans.rotate.y = -45;
            trans.scale.set(0.5f, 0.5f, 0.5f);
            figure1->setTransform(&trans);
        }

        {
            state->clearColorf(1, 0, 0, 0);
            state->clear();
            state->viewport(0, 0, offscreen->getWidth(), offscreen->getHeight());
            renderer->rendering(device, figure, figure0);
            renderer->rendering(device, figure, figure1);
        }
    }
    offscreen->unbind(state);

    {
        state->viewport(0, 0, getWindowSize().x, getWindowSize().y);
        MSpriteManager spriteManager = getSpriteManager();
        MTextureImage texture = offscreen->getColorTexture();

        spriteManager->renderingImage(texture, 0, texture->getHeight(), texture->getWidth(), -texture->getHeight(), 0, 0, texture->getWidth() * 2, texture->getHeight() * 2);
//        spriteManager->renderingImage(texture, texture->getWidth(), texture->getHeight(), -texture->getWidth(), -texture->getHeight());
    }
    getWindowDevice()->postFrontBuffer();
}

/**
 * 新規タスクの実行をリクエストした
 */
jcboolean BenchmarkApplication::onAppTask(const ApplicationTaskContext &task) {
    switch (task.uniqueId) {
        case BenchmarkTask_LoadTexture:
            loadTexture(platformContext->createSlaveDevice());
            return jctrue;
        default:
            // ハンドル出来なかった
            return jcfalse;
    }
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

    texture.reset();
    figure.reset();
    shader.reset();
}

}

