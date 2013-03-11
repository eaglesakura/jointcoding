/*
 * egllog.h
 *
 *  Created on: 2012/05/12
 *      Author: Takeshi
 */

#ifndef EGLLOG_H_
#define EGLLOG_H_

#include    "string.h"

// SmartPtrクラスのログを出力
//#define __ENABLELOG_SMARTPTR


namespace jc {

/**
 * __FILE__マクロからファイル名を取得する
 */
inline charactor* __getFileName(const charactor* __file__) {
#if ( defined(BUILD_MacOSX) || defined(BUILD_Android) || defined(BUILD_MacOSX) )
    return strrchr(__file__, '/') + 1;
#else
    return strrchr(__file__, '/') + 1;
#endif
}

enum LogType_e {
    /**
     * デバッグ中のみ表示したい
     */
    LogType_Debug,

    /**
     * 特に重要なログで表示する
     */
    LogType_Alert,
};

#ifndef LOG_TAG
#define LOG_TAG "jni-log"
#endif

void __logDebugF(const LogType_e type, const charactor* __file, const charactor* fmt, ...);

#if defined(DEBUG) || defined(OUTPUT_LOG)

/**
 * 特定条件下の設定
 */
#define jclog_from_f(file, line, fmt, ... )       { ::jc::__logDebugF(::jc::LogType_Debug, ::jc::__getFileName(file), "L %d | " fmt, line, __VA_ARGS__); }

/**
 * フォーマット付きログ
 */
#define jclogf(fmt, ... )		{ ::jc::__logDebugF(::jc::LogType_Debug, ::jc::__getFileName(__FILE__), "L %d | " fmt, __LINE__, __VA_ARGS__); }

/**
 * 単純ログ
 */
#define jclog( msg )		    { ::jc::__logDebugF(::jc::LogType_Debug, ::jc::__getFileName(__FILE__), "L %d | " msg, __LINE__); }

/**
 * アラート表示
 */
#define jcalert( msg )      { ::jc::__logDebugF(::jc::LogType_Alert, ::jc::__getFileName(__FILE__), "L %d | " msg, __LINE__); }

/**
 * フォーマット付きアラート
 */
#define jcalertf( fmt, ... )      { ::jc::__logDebugF(LogType_Alert, ::jc::__getFileName(__FILE__), "L %d | " fmt, __LINE__, __VA_ARGS__); }

#else

/**
 * 特定条件下の設定
 */
#define jclog_from_f(...)       {  }

/**
 * フォーマット付きログ
 */
#define jclogf( ... )       {  }

/**
 * 単純ログ
 */
#define jclog( ... )            {  }

/**
 * アラート表示
 */
#define jcalert( ... )      {  }

/**
 * フォーマット付きアラート
 */
#define jcalertf( ... )      {  }


#endif

}
#endif /* EGLLOG_H_ */
