/*
 * jcPlatform.cpp
 *
 *  Created on: 2012/09/25
 */

#include    "jcandroid/platform/PlatformImpl.h"

namespace ndk {

namespace {
/**
 * プラットフォーム固有のコンテキスト
 */
static MNativeContext __g_NativeContext;
}

void NativeContext_initPlatform(MNativeContext context) {
    __g_NativeContext = context;
}

MNativeContext& NativeContext_get() {
    return __g_NativeContext;
}

}

namespace jc {

/**
 * プラットフォームごと特有のバージョン番号を取得する。
 */
s32 Platform::getPlatformVersionNumber() {
    return AndroidAPI_Develop;
}

/**
 * ファイルシステムアクセスクラスを取得する。
 */ //
jc_sp<FileSystem> Platform::getFileSystem() {
    static jc_sp<FileSystem> g_fileSystem( new ndk::NDKFileSystem(ndk::NativeContext_get()));
    return g_fileSystem;
}

/**
 * ネイティブAPIコールが可能なスレッドを作成する。
 */
Thread* Platform::newThread(MRunnable runnable) {
    return new ndk::JavaJointThread(runnable);
}

/**
 * GCを行う。
 * プラットフォーム固有の管理メモリ等を解放する
 */
void Platform::gc() {
    ndk::NativeContext::gc();
}
}
