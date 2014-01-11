/*
 * esBenchmarkApplication.cpp
 *
 *  Created on: 2013/05/06
 */
#include    "TextureAtlasData.pb.h"
#include    <google/protobuf/io/coded_stream.h>

#include    "esBenchmarkApplication.h"
#include    "jc/collection/ArrayHandle.hpp"

#include    "jc/framework/assets/FigureAsset.hpp"
#include    "jc/graphics/figure/data/ArchiveFigureDataFactory.h"

#include    "jc/framework/assets/figure/FigureLoader.h"
#include    "jc/gl/surface/RenderingSurfaceStub.hpp"

using namespace jc::fw;
namespace es {

BenchmarkApplication::BenchmarkApplication() {
    rotate = 0;
    initialized = jcfalse;
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
//    specs.surfaceFormat = PixelFormat_RGB888;
//    specs.extensions.enable(SurfaceSpecExtension_AndroidTextureView);
//    specs.extensions.enable(SurfaceSpecExtension_AndroidSurfaceViewOnTop);
    return specs;
}

/**
 * レンダリングサーフェイスが変更された
 */
void BenchmarkApplication::onRenderingSurfaceChanged(RenderingContext *pContext, const MRenderingSurface old, const MRenderingSurface now) {
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

    MDevice device = getWindowDevice();
    device->getState()->blendEnable(jctrue);
    device->getState()->blendFunc(GLBlendType_Alpha);

    {
        spriteManager = SpriteManager::createInstance(getRenderingContext(), getWindowDevice());
    }

#if 0
    {
        MFileMapper file = Platform::getFileSystem()->loadFile(Uri::fromAssets("atlas.pb"), NULL);
        jc_sp<jc_res_atlas::AtlasGroup> group(new jc_res_atlas::AtlasGroup());


        google::protobuf::io::CodedInputStream is((uint8_t*)file->getHead(), (int)file->length());
//        google::protobuf::io::CodedInputStream is((google::protobuf::internal::uint8*) file->getHead(), file->length());

        jclogf("created image size(%d)", group->images_size());

        group->MergeFromCodedStream(&is);

        jclogf("loaded image size(%d)", group->images_size());
        jclogf("file name(%s)", group->filename().c_str());
    }
#endif

//    // テクスチャロードを開始する
    startNewtask(BenchmarkTask_LoadTexture, 0);
    startNewtask(BenchmarkTask_LoadResources, 0);
}

void BenchmarkApplication::loadTexture(MDevice subDevice) {
    try {
        jclog("lock start");
        DeviceLock lock(subDevice, jctrue);

        texture = TextureImage::decode(subDevice, Uri::fromAssets("images/test.png"), PixelFormat_RGBA8888);
        texture->unbind(subDevice->getState());

        jclog("load finish");
        glFinish();
    } catch (Exception &e) {
        jcloge(e);
    }
}

void BenchmarkApplication::loadResource(MDevice subDevice) {

//    Thread::sleep(1000 * 3);
    try {
        MDevice device = subDevice;
        DeviceLock lock(device, jctrue);

        // スキニングなしフィギュアをロード
        {
            susanow.reset(new Figure());

            // フィギュアロード
            jc_sp<ArchiveFigureDataFactory> factory(new ArchiveFigureDataFactory());
            jc_sp<UriTextureFactory> texFactory(new UriTextureFactory());
            texFactory->fromAssets("");
            texFactory->setFilenameExt(".png");

            factory->initialize(Uri::fromAssets("susanow.archive"));

            jc_sp<FigureLoader> loader(new FigureLoader(device, factory, texFactory));

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

            jc_sp<FigureLoader> loader(new FigureLoader(device, factory, texFactory));

            // 読み込み対象を指定する
            loader->setLoadTarget(antan);

            // 読込を行う
            loader->load();
        }
        // シャドウ用のレンダラを確保する
        {
            // シェーダー読込
            MGLShaderProgram shader = ShaderProgram::buildFromUri(device, Uri::fromAssets("shadow.vert"), Uri::fromAssets("shadow.frag"));
            assert(shader);

            shadowRenderer.reset(new ShadowmapRenderer());
            shadowRenderer->initialize(device, shader);
        }

        // フィギュアのインスタンスを確保する
        {
            // シェーダー読込
            MGLShaderProgram shader = ShaderProgram::buildFromUri(device, Uri::fromAssets("basic.vert"), Uri::fromAssets("basic.frag"));
            assert(shader);

            basicShader = shader;
            renderer.reset(new BasicFigureRenderer());
            renderer->initialize(device, shader);

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

        // オフスクリーンターゲットを生成
        {
            const s32 width = 2048;
            const s32 height = width;
            shadowmap.reset(new FrameBufferObject(device));
            // オフスクリーンのリサイズを行う
            shadowmap->bind(device->getState());
            shadowmap->resize(device->getState(), width, height);

            if (shadowmap->allocDepthRenderTexture(device)) {
                jclog("alloc depth texture");
                shadowmapTexture = shadowmap->getDepthTexture();
            } else {
                jclog("bad alloc depth texture");
                shadowmap->allocColorRenderTexture(device, PixelFormat_LuminanceF16);
                shadowmap->allocDepthRenderbuffer(device, 24);

                shadowmapTexture = shadowmap->getColorTexture();
            }
            // オフスクリーンテクスチャの確保を行う
            //        shadowmap->allocDepthRenderTexture(device);
            shadowmap->checkFramebufferStatus();
            shadowmap->unbind(device->getState());

            assert(glIsFramebuffer(shadowmap->getName()));
        }
    } catch (Exception &e) {
        jcloge(e);
        throw;
    }

    try {
        MDevice device = getWindowDevice();
        DeviceLock lock(device, jctrue);

        // デバイスを委譲する
        shadowmap->setRenderDevice(device);
    } catch (Exception &e) {
        jcloge(e);
    }

    initialized = jctrue;
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
    if (!initialized) {
        state->clearColorf(1, (float) (jc::randDouble()), 0, 1);
        state->clearSurface();

        device->postFrontBuffer();
        return;
    }

    {
        state->viewport(0, 0, getPlatformViewSize().x, getPlatformViewSize().y);
        state->clearColorf(0, 1, 1, 1);
        state->clearSurface();
    }
//

    {
        state->depthTestEnable(jcfalse);
        renderingContext->viewportVirtual();
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
            m.rotateY(-rotate);
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
        basicShader->bind(device->getState());
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

    // サーフェイスサイズを仮想ディスプレイから通常ディスプレイに変換する
    getRenderingContext()->pushSurface(RenderingSurfaceStub::createInstance(getPlatformViewSize()));
    {
        MTextureImage texture = shadowmapTexture;
        spriteManager->renderingImage(texture, 0, texture->getHeight(), texture->getWidth(), -texture->getHeight(), 0, 0, 256, 256);
    }
    getRenderingContext()->popSurface();

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
        case BenchmarkTask_LoadResources:
            loadResource(platformContext->createSlaveDevice());
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
    jclogf("onAppDestroy %x", this);
}

/**
 * 全てのタスクが終了し、タスク数が0に達した
 * メモリのクリーンアップを行うことを想定し、必ずスレイブデバイスを入力する
 */
void BenchmarkApplication::onTaskDestroyed(const ApplicationTaskContext &lastTask) {
    jclog("all task finished");

    try {
        DeviceLock lock(getWindowDevice(), jctrue);

        texture.reset();
        susanow.reset();
        shadowmap.reset();
        basicShader.reset();
    } catch (Exception &e) {

    }
}

}

