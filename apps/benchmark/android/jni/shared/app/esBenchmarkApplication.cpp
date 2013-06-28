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
 * レンダリングサーフェイスが変更された
 */
void BenchmarkApplication::onRenderingSurfaceChanged(RenderingContext *pContext, const MRenderingSurface old, const MRenderingSurface now) {
    spriteManager->setSurfaceAspect(now->getWidth(), now->getHeight());
}

/**
 * アプリ初期化を行わせる
 * メソッド呼び出し時点でデバイスロック済み。
 */
void BenchmarkApplication::onAppInitialize() {
    jclogf("onAppInitialize 0x%x", this);

    // 仮想ディスプレイサイズをHDに変更する
    {
        renderingContext->getVirtualDisplay()->setVirtualDisplaySize(DISPLAYSIZE_RETINA_iP5.x, DISPLAYSIZE_RETINA_iP5.y);
    }

    getWindowDevice()->getState()->blendEnable(jctrue);
    getWindowDevice()->getState()->blendFunc(GLBlendType_Alpha);

    {
        spriteManager = SpriteManager::createInstance(getRenderingContext()->getDevice());
        spriteManager->setSurfaceAspect((u32) renderingContext->getVirtualDisplaySize().x, (u32) renderingContext->getVirtualDisplaySize().y);
    }

    // スキニングなしフィギュアをロード
    {
        susanow.reset(new Figure());

        // フィギュアロード
        jc_sp<ArchiveFigureDataFactory> factory(new ArchiveFigureDataFactory());
        jc_sp<UriTextureFactory> texFactory(new UriTextureFactory());
        texFactory->fromAssets("");
        texFactory->setFilenameExt(".png");

        factory->initialize(Uri::fromAssets("susanow.archive"));

        jc_sp<FigureLoader> loader(new FigureLoader(getWindowDevice(), factory, texFactory));

        // 読み込み対象を指定する
        loader->setLoadTarget(susanow);

        // 読込を行う
        loader->load();
    }

    // スキニング付きフィギュアをロード
    {
        antan.reset(new Figure());

        // フィギュアロード
        jc_sp<ArchiveFigureDataFactory> factory(new ArchiveFigureDataFactory());
        jc_sp<UriTextureFactory> texFactory(new UriTextureFactory());
        texFactory->fromAssets("");
        texFactory->setFilenameExt(".jpg");

        factory->initialize(Uri::fromAssets("antan.archive"));

        jc_sp<FigureLoader> loader(new FigureLoader(getWindowDevice(), factory, texFactory));

        // 読み込み対象を指定する
        loader->setLoadTarget(antan);

        // 読込を行う
        loader->load();
    }

    // フィギュアのインスタンスを確保する
    {
        // シェーダー読込
        MGLShaderProgram shader = ShaderProgram::buildFromUri(getWindowDevice(), Uri::fromAssets("basic.vert"), Uri::fromAssets("basic.frag"));
        assert(shader);

        basicShader = shader;
        renderer.reset(new BasicFigureRenderer());
        renderer->initialize(getWindowDevice(), shader);

        worldEnv.reset(new EnvironmentInstanceState());

        {
            susanow_instance.reset(renderer->createInstanceState(susanow));
            susanow_instance->setEnvironmentState(worldEnv);
        }

        {
            antan_instance.reset(renderer->createInstanceState(antan));
            antan_instance->setEnvironmentState(worldEnv);
        }
    }
    // シャドウ用のレンダラを確保する
    {
        // シェーダー読込
        MGLShaderProgram shader = ShaderProgram::buildFromUri(getWindowDevice(), Uri::fromAssets("shadow.vert"), Uri::fromAssets("shadow.frag"));
        assert(shader);

        shadowRenderer.reset(new ShadowmapRenderer());
        shadowRenderer->initialize(getWindowDevice(), shader);
    }

    // オフスクリーンターゲットを生成
    {
        MDevice device = getWindowDevice();

        const s32 width = 2048;
        const s32 height = width;
        shadowmap.reset(new FrameBufferObject(device));
        // オフスクリーンのリサイズを行う
        shadowmap->resize(device->getState(), width, height);

        if (shadowmap->allocDepthRenderTexture(device)) {
            shadowmapTexture = shadowmap->getDepthTexture();
        } else {
            shadowmap->allocColorRenderTexture(device, PixelFormat_LuminanceF16);
            shadowmap->allocDepthRenderbuffer(device, 24);

            shadowmapTexture = shadowmap->getColorTexture();
        }
        // オフスクリーンテクスチャの確保を行う
//        shadowmap->allocDepthRenderTexture(device);
        shadowmap->checkFramebufferStatus();
        shadowmap->unbind(device->getState());
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
    MDevice device = getWindowDevice();
    MGLState state = device->getState();
    {
        state->viewport(0, 0, getPlatformViewSize().x, getPlatformViewSize().y);
        state->clearColorf(0, 1, 1, 1);
        state->clearSurface();
    }
//

    {
        state->depthTestEnable(jcfalse);
        renderingContext->viewportVirtual();
        spriteManager->setSurfaceAspect((u32)renderingContext->getVirtualDisplaySize().x, (u32)renderingContext->getVirtualDisplaySize().y);
        spriteManager->renderingRect(0, 0, renderingContext->getVirtualDisplaySize().x, renderingContext->getVirtualDisplaySize().y, 0x7FFFFFFF);
    }
//    {
//        spriteManager->renderingArea(createRectFromXYWH<float>(1, 1, 512, 512), 0xFFFF00FF, 0x0000FFFF);
//
//        if (texture) {
//            spriteManager->renderingImage(texture, 128, 128, Color::fromRGBAf(1, 1, 1, 0.75f));
//
//            static float rotate = 0;
//            spriteManager->renderingImage(texture, createRectFromXYWH<float>(256, 256, texture->getWidth(), texture->getHeight()), rotate += 1, Color::fromRGBAf(1, 1, 1, 0.5f));
//        }
//    }

// rendering 3d

// figureやライトの位置を更新する
    {
        rotate = jc::normalizeDegree(rotate += 1.0f);

        // ライト位置決定
        {
            Vector3f basicLightPos(0, 200, 200);
            Matrix4x4 m;
//            m.rotateY(-rotate);
//            m.rotateX(rotate);
            m.multiply(basicLightPos, &basicLightPos);
            worldEnv->getShadowmapLight()->setDirection(basicLightPos, Vector3f(0, 0, 0));

            MCamera shadowCamera = worldEnv->getShadowmapLight()->getShadowCamera();
            shadowCamera->setProjection(100.0f, 500.0f, shadowmap->getAspect());
        }

        {
            Vector3f camPos(0, 5, 200);
            Matrix4x4 m;
            m.rotateY(rotate);
            m.multiply(camPos, &camPos);
            worldEnv->getMainCamera()->lookAt(camPos, Vector3f(0, -5, 0), Vector3f(0, 1, 0));
            worldEnv->getMainCamera()->projection(100.0f, 350.0f, 45.0f, renderingContext->getVirtualDisplayAspect());
        }
        {
            jc::Transform trans;
            trans.translate.x = 15;
            susanow_instance->setTransform(&trans);
        }
        {
            jc::Transform trans;
            trans.translate.x = -30;
            trans.rotate.y = -45;
//            trans.scale.set(0.5f, 0.5f, 0.5f);
            antan_instance->setTransform(&trans);
        }
    }

    // シャドウマップレンダリング
    renderingContext->pushSurface(shadowmap);
    shadowmap->bind(state);
    {
        state->clearColorf(1, 1, 1, 1);
        state->clear();
        state->viewport(0, 0, shadowmap->getWidth(), shadowmap->getHeight());

        state->colorMask(jctrue, jcfalse, jcfalse, jcfalse);
        shadowRenderer->rendering(device, susanow, susanow_instance);
        shadowRenderer->rendering(device, antan, antan_instance);
        state->colorMask(jctrue, jctrue, jctrue, jctrue);

    }
    shadowmap->unbind(state);
    renderingContext->popSurface();

    // 通常レンダリング
    {
        renderingContext->viewportVirtual();
        basicShader->bind();
        {
            TextureUniform unif;
            unif.setLocation(basicShader, "unif_shadowmap");
//            assert(unif.valid());
            unif.upload(state, shadowmapTexture);
        }

        {
            Matrix4Uniform unif;
            unif.setLocation(basicShader, "unif_shadow_wlp");
//            assert(unif.valid());

            Matrix4x4 wlp;
            worldEnv->calcShadowWorldLoopProjection(susanow_instance->getModelview(), &wlp);
            unif.upload(wlp, GL_FALSE);
        }
        renderer->rendering(device, susanow, susanow_instance);

        {
            Matrix4Uniform unif;
            unif.setLocation(basicShader, "unif_shadow_wlp");
//            assert(unif.valid());

            Matrix4x4 wlp;
            worldEnv->calcShadowWorldLoopProjection(antan_instance->getModelview(), &wlp);
            unif.upload(wlp, GL_FALSE);
        }
        renderer->rendering(device, antan, antan_instance);
    }

    {
        state->viewport(0, 0, getPlatformViewSize().x, getPlatformViewSize().y);
        MTextureImage texture = shadowmapTexture;
        spriteManager->setSurfaceAspect(getPlatformViewSize().x, getPlatformViewSize().y);
        spriteManager->renderingImage(texture, 0, texture->getHeight(), texture->getWidth(), -texture->getHeight(), 0, 0, 256, 256);
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
    susanow.reset();
    shadowmap.reset();
    basicShader.reset();
}

}

