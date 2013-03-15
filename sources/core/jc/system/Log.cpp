/*
 * JCLog.cpp
 *
 *  Created on: 2012/06/14
 *      Author: Takeshi
 */
#include    "jointcoding.h"

#ifdef  BUILD_Android
#include    "android/log.h"
#endif

#ifdef BUILD_iOS

#endif

namespace jc {

void __logDebugF(const LogType_e type, const charactor* __file, const char* fmt, ...) {
#if defined(DEBUG) || defined(OUTPUT_LOG)
//    static jc::jcmutex mutex;
//    MutexLock lock(mutex);
    {
#ifndef IDE_ECLIPSE
        va_list ap;
        va_start(ap, fmt);
#ifdef  BUILD_Android
        {
            static const s32 LOG_TYPES[] = {
#ifdef  DEBUG
                    ANDROID_LOG_DEBUG,
#else
                    ANDROID_LOG_INFO,
#endif
                    ANDROID_LOG_ERROR,
            };
            __android_log_vprint(LOG_TYPES[type], __file, fmt, ap);
        }
#endif // endif ANDROID
#if defined(BUILD_iOS) || defined(BUILD_MacOSX)
        {
            printf("%s | ", __file); // ファイル名出力
            vprintf(fmt, ap);// 通常ログ出力
            printf("\n");// コンソール改行
        }
#endif
        va_end(ap);
#endif
    }
#endif
}

}
