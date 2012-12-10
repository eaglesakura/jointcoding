/*
 * AppFragmentImpl.cpp
 *
 *  Created on: 2012/09/19
 */

#include    "jointcoding-android.h"
#include    "jcGL.h"
#include    "jcThread.h"
#include    "jcRunnable.h"
#include    "jcThreadUtil.h"
#include    "AppFragment.h"
#include    "ndkNativeFragmentBase.h"
#include    "jcglDevice.h"
#include    "ndkViewUtil.h"
#include    "jcglDeviceLock.h"
#include    "jcglQuad.h"
#include    "jcglShaderProgram.h"
#include    "jcglTextureImage.h"
#include    "cpu-features.h"
#include    "jcglSpriteManager.h"
#include    "jcViewOnTouchListener.h"
#include    "jcViewOnClickListener.h"

using namespace ndk;
using namespace jc;
using namespace jc::gl;

class GLTouchListener: public Object, public ViewOnTouchListener, public ViewOnClickListener {
public:
    GLTouchListener() {

    }

    virtual ~GLTouchListener() {
//        jclogf("destroy GLTouchListener %x", this);
    }

    /**
     * タッチイベントを開始した
     */
    virtual void onTouchEventBegin(const s32 eventNum) {
//        jclogf("event-begin %d", eventNum);
    }

    /**
     * タッチイベントの処理を行う。
     */
    virtual void onTouchEvent(jc_sp<TouchEventProtocol> event, const s32 index, const s32 eventNum) {
//        jclogf("action(%d) %d / %d v(%f, %f)", event->getEventType(), index, eventNum, event->getEventPosX(), event->getEventPosY());
        }

        /**
         * タッチイベントが完了した
         */
        virtual void onTouchEventComplete() {
//        jclog("event-complete");
        }

        virtual void onClick( ) {
            jclog("onClick");
        }
    };

class AppFragmentNative: public NativeFragmentBase {

    jcboolean initialized;
public:
    ndk::JClass view;

    /**
     * レンダリングするテクスチャ
     */
    jc::gl::MTextureImage texture;

    MSpriteManager spriteManager;

    static void backRender(AppFragmentNative* fragment) {
        MDevice device = ViewUtil::getDeviceFromSurfaceView(fragment->view.getObject());

        try {

            // 初期化を行う
            {
                fragment->initialize(device);
            }

            while (true) {
                device->waitLockRequest(10, NULL);
                DeviceLock lock(device, false);

                // ロックに失敗したため、レンダリングループを終了する
                if (!lock.isLockCompleted()) {
                    return;
                }

                glClearColor(0, 1, 1, 1);
                glClear(GL_COLOR_BUFFER_BIT);

                // 矩形描画
//                fragment->texture->bind();
//                fragment->quad->rendering();

                if (fragment->texture) {
                    fragment->spriteManager->rendering(fragment->texture, 0, 0, 300, 300, 0, 0, 300, 300);
                    fragment->spriteManager->rendering(fragment->texture, 300, 0, 300, 300, 300, 0, 300, 300);

                    fragment->spriteManager->rendering(fragment->texture, 0, 300, 300, 300, 0, 300, 300, 300);
                    fragment->spriteManager->rendering(fragment->texture, 300, 300, 300, 300, 300, 300, 300, 300);
                }
                //                fragment->spriteManager->rendering(fragment->texture, 500, 100, 100, 100);

                device->postFrontBuffer();
            }
        } catch (Exception &e) {
            jcloge(e);
        }
    }
    AppFragmentNative() {
        jclogf("new AppFragmentNative() %x", this);
        initialized = jcfalse;
    }

    virtual ~AppFragmentNative() {
        jclogf("delete AppFragmentNative() %x", this);
    }

    /**
     * 必要な資源の確保を行う
     */
    void initialize(MDevice device) {
        if (initialized) {
            return;
        }

// ロックを取得する
        DeviceLock lock(device, true);

        device->getState()->syncContext();

// マネージャ作成
        spriteManager = SpriteManager::createInstance(device);

// テクスチャの作成
        try {
            texture = TextureImage::decode(device, Uri::fromAssets("ic_launcher.jpg"), PixelFormat_RGB888);
            texture->bind();
            {
                texture->setMagFilter(GL_LINEAR);
                texture->setMinFilter(GL_LINEAR);
            }
            texture->unbind();
            jclogf("texture :: %x", texture.get());
        } catch (IOException &ioe) {
            jcloge(ioe);
        }
        initialized = jctrue;
    }

    /**
     * 資源の解放を行う
     */
    void destroy(MDevice device) {
        device->dispose();
    }

    static void startRendering(jc_sp<NativeFragmentBase> fragment ) {
        ::jc::ThreadUtil::asyncFunction((jc::data_runnable_function)AppFragmentNative::backRender, fragment, "rendering");
    }
};

#define fragment    native_fragment_ptr(AppFragmentNative, _this)

extern "C" {

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_AppFragment_onEGLPauseBegin(JNIEnv *env, jobject _this, jobject arg0) {
// call env reset
    initJniEnv(env);

// add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_sample_gl2_AppFragment_onEGLPauseBegin");

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_AppFragment_onEGLPauseCompleted(JNIEnv *env, jobject _this, jobject arg0) {
// call env reset
    initJniEnv(env);

// add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_sample_gl2_AppFragment_onEGLPauseCompleted");

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_AppFragment_onEGLResumeCompleted(JNIEnv *env, jobject _this, jobject arg0) {
// call env reset
    initJniEnv(env);

// add code.
    jclogf("call method!! :: %s", "Java_com_eaglesakura_jc_sample_gl2_AppFragment_onEGLResumeCompleted");
    AppFragmentNative::startRendering(native_fragment(_this));

    return;
}

// main
JNIEXPORT void JNICALL Java_com_eaglesakura_jc_android_sample_hello_AppFragment_onEGLInitializeCompleted(JNIEnv *env, jobject _this, jobject view) {
// call env reset
    initJniEnv(env);

    fragment ->view.reset(view).addGlobalRef();

    jc_sp<Object> listener(new GLTouchListener());
    try {
//        ViewUtil::setOnTouchListener(view, listener);
        ViewUtil::setOnClickListener(view, listener);
    } catch (Exception &e) {
        jcloge(e);
    }

    return;
}

// main
JNIEXPORT jint JNICALL Java_com_eaglesakura_jc_android_sample_hello_AppFragment_newState(JNIEnv *env, jobject _this) {
// call env reset
    initJniEnv(env);

    jclogf("has neon ? %x", android_getCpuFeatures() & ANDROID_CPU_ARM_FEATURE_NEON);

    AppFragmentNative *state = new AppFragmentNative();
    return (jint) new jc_sp<jc::Object>(state);
}

}
