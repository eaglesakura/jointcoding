/*
 * jcPlatform.h
 *
 *  Created on: 2012/09/25
 */

#ifndef JCPLATFORM_H_
#define JCPLATFORM_H_

#include    "jcFileSystem.h"
#include    "jcThread.h"

namespace jc {

// Android版のplatform定数
#ifdef BUILD_Android
enum Platform_e {
    /**
     * Androidで実行されている
     */
    Platform_Android,
};
#endif // BUILD_Android
// iOS版のplatform定数
#ifdef  BUILD_iOS
enum Platform_e {
    /**
     * iPhoneで実行されている
     */
    Platform_iPhone,

    /**
     * iPadで実行されている
     */
    Platform_iPad,

};
#endif // BUILD_iOS
#ifdef  BUILD_Android
/**
 * Androidのバージョン定数を示す。
 * APIレベルごとに定義する。
 */
enum AndroidAPI_e {
    /**
     * Android 4.0.2
     */
    AndroidAPI_API14_ICS = 14,

    /**
     * Android 4.0.3
     */
    AndroidAPI_API15_ICS = 15,

    /**
     * Android 4.1
     */
    AndroidAPI_API16_JB = 16,

    /**
     * 開発に利用したプラットフォームバージョン
     */
    AndroidAPI_Develop = AndroidAPI_API14_ICS,
};

#endif // BUILD_Android
#ifdef  BUILD_iOS
/**
 * iOSのバージョン定数を示す。
 * 上の桁がメジャーバージョン、下の桁がマイナーバージョン
 */
enum iOSAPI_e {
    /**
     * iOS前バージョン
     */
    iOSAPI_051 = 51,

    /**
     * iOS現バージョン
     */
    iOSAPI_060 = 60,

    /**
     * iOS版の対象バージョン
     */
    iOSAPI_Develop = iOSAPI_060,
};
#endif // BUILD_iOS
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
    static s32 getPlatformVersionNumber();

    /**
     * ネイティブAPIコールが可能なスレッドを作成する。
     */
    static Thread* newThread(MRunnable runnable);

};

}

#endif /* JCPLATFORM_H_ */
