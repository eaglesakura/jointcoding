/*
 * jcPlatform.h
 *
 *  Created on: 2012/09/25
 */

#ifndef JCPLATFORM_H_
#define JCPLATFORM_H_

#include    "jc/io/FileSystem.h"
#include    "jc/thread/Thread.h"

namespace jc {

enum Platform_e {
    /**
     * Androidで実行されている
     */
    Platform_Android,

    /**
     * iOSで実行されている
     */
    Platform_iOS,
};

/**
 * プラットフォーム固有番号を示す
 */
enum PlatformApiVersion_e {
    /**
     * Android 4.0.2
     */
    AndroidAPI_API14_ICS = 0x010000 + 14,

    /**
     * Android 4.0.3
     */
    AndroidAPI_API15_ICS,

    /**
     * Android 4.1
     */
    AndroidAPI_API16_JB,

    /**
     * Android 4.2
     */
    AndroidAPI_API17_JB,

    /**
     * Android 4.3
     */
    AndroidAPI_API18_JB,

    /**
     * Android 4.4
     */
    AndroidAPI_API19_KK,

    /**
     * 開発に利用したプラットフォームバージョン
     */
    AndroidAPI_Develop = AndroidAPI_API14_ICS,

    /**
     * iOS現在バージョン
     */
    iOSAPI_060 = 0x020000 + 60,

    /**
     * iOS 7.0
     */
    iOSAPI_070 = 0x020000 + 70,

    /**
     * iOS版の対象バージョン
     */
    iOSAPI_Develop = iOSAPI_060,
};

/**
 * プラットフォーム固有処理を行う
 */
class Platform {
    // 生成禁止
    Platform();
    // 廃棄禁止
    ~Platform();
public:
    /**
     * ファイルシステムアクセスクラスを取得する。
     */
    static jc_sp<FileSystem> getFileSystem();

    /**
     * プラットフォームごと特有のバージョン番号を取得する。
     */
    static PlatformApiVersion_e getPlatformVersionNumber();

    /**
     * ネイティブAPIコールが可能なスレッドを作成する。
     */
    static Thread* newThread(MRunnable runnable);

    /**
     * GCを行う。
     * プラットフォーム固有の管理メモリ等を解放する
     */
    static void gc();
};

}

#endif /* JCPLATFORM_H_ */
