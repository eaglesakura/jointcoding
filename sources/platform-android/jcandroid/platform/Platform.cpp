/*
 * jcPlatform.cpp
 *
 *  Created on: 2012/09/25
 */

#include    "jcandroid/platform/PlatformImpl.h"

namespace {
/**
 * プラットフォーム固有のコンテキスト
 */
static MNativeContext g_context;

}

namespace ndk {

void initPlatform(MNativeContext context) {
    g_context = context;
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
    static jc_sp<FileSystem> g_fileSystem( new ndk::NDKFileSystem(g_context));
    return g_fileSystem;
}

/**
 * ネイティブAPIコールが可能なスレッドを作成する。
 */
Thread* Platform::newThread(MRunnable runnable) {
    return new ndk::JavaJointThread(runnable);
}

}
