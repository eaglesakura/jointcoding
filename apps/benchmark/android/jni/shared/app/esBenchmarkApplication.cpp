/*
 * esBenchmarkApplication.cpp
 *
 *  Created on: 2013/05/06
 */

#include    "esBenchmarkApplication.h"
#include    "jc/collection/ArrayHandle.hpp"

#include    "jc/framework/assets/figure/FigureAsset.hpp"
#include    "jc/graphics/figure/data/ArchiveFigureDataFactory.h"

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
        factory->initialize(Uri::fromAssets("susanow.archive"));

        jc_sp<FigureLoader> loader(new FigureLoader(getWindowDevice(), factory));

        // 読み込み対象を指定する
        loader->setLoadTarget(figure);

        // 読込を行う
        loader->load();
    }

    {
        // シェーダー読込
        shader = ShaderProgram::buildFromUri(getWindowDevice(), Uri::fromAssets("basic.vshader"), Uri::fromAssets("basic.fshader"));
        assert(shader);

        unif_wlp.setLocation(shader, "unif_wlp");
    }

//    // テクスチャロードを開始する
//    startNewtask(BenchmarkTask_LoadTexture, 0);
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
    }

//    {
//        MSpriteManager spriteManager = getSpriteManager();
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
    {
        MGLState state = getWindowDevice()->getState();
        shader->bind();
        state->depthTestEnable(jctrue);
        Matrix4x4 world;
        Matrix4x4 look;
        Matrix4x4 prj;
        {
            world.rotateY(rotate += 1);
            rotate = jc::normalizeDegree(rotate);
            look.lookAt(Vector3f(100, 25, 150), Vector3f(0, 0, 0), Vector3f(0, 1, 0));
            prj.projection(50.0f, 500.0f, 45.0f, getWindowDevice()->getSurfaceAspect());
        }

        // world loop projection行列を生成する
        Matrix4x4 wlp;
        multiply(world, look, &wlp);
        multiply(wlp, prj, &wlp);

        // アップロードする
        unif_wlp.upload(wlp);

        Attribute pos;
        pos.setLocation(shader, "attr_pos");

        Attribute uv;
        uv.setLocation(shader, "attr_uv");

        Attribute normal;
        normal.setLocation(shader, "attr_normal");

        assert(pos.valid());

        for (s32 i = 0; i < figure->getNodeNum(); ++i) {
            FigureNodeHandle node = figure->getNodeHandle(i);
            for (s32 k = 0; k < node->getFragmentNum(); ++k) {
                NodeFragment *fragment = node->getFragment(k);
                for (s32 c = 0; c < fragment->getContextNum(); ++c) {
                    MeshContext *context = fragment->getContext(c);

                    // 各種バッファを取り出す
                    VertexBufferObject<void> *vbo = context->getVertexBuffer();
                    IndexBufferObject *ibo = context->getIndicesBuffer();

                    // 属性を設定する
                    vbo->bind(state);
                    ibo->bind(state);
                    {
                        pos.attribute(state, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), NULL, 0);
                        uv.attribute(state, 2, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), NULL, sizeof(Vector3f));
                        normal.attribute(state, 3, GL_FLOAT, GL_FALSE, sizeof(BasicVertex), NULL, sizeof(Vector3f) + sizeof(Vector2f));

                        ibo->rendering(GL_TRIANGLES);
                    }
                    vbo->unbind(state);
                    ibo->unbind(state);
                }
            }
        }
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

