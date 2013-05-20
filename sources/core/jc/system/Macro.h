/*
 * jcMacro.h
 * グローバルで必要なマクロを定義する
 *  Created on: 2012/07/21

 */

#ifndef JCMACRO_H_
#define JCMACRO_H_


/**
 * BUILD_XXXでプラットフォームを示す。
 *
 * BUILD_Android
 * BUILD_iOS
 * BUILD_MacOSX
 */
// ECLIPSE認識のため、デフォルトで__ANDROID__チェックを行う

#ifndef BUILD_MacOSX
    #ifndef __APPLE__
        #ifndef __ANDROID__
            #define __ANDROID__
        #endif  // __ANDROID__
    #endif  // __APPLE__
    // 定数設定
    #ifdef  __APPLE__
        #define BUILD_iOS 1
    #endif

    // ANDROIDモード
    #ifdef  __ANDROID__
        #ifndef BUILD_Android
            #define BUILD_Android 1
        #endif

        #ifdef  __ARM_V7__
            #define BUILD_Android_ARMv7 1
        #else
            #define BUILD_Android_ARMv6 1
        #endif
    #endif

#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif // NULL


#ifndef BUILD_Android
/**
 * NDK_DEBUG=1の場合、NDEBUGフラグが立たないためDEBUGを立てる。
 */
#ifndef NDEBUG
#ifndef DEBUG
    #define DEBUG
#endif
#endif

#else

#ifndef NDEBUG
#define DEBUG
#endif

#endif

/**
 * 安全にdeleteを行う
 */
#define     SAFE_DELETE( p )        if( p ){    delete p; p = NULL;     }

/**
 * 安全に配列deleteを行う
 */
#define     SAFE_DELETE_ARRAY( p )  if( p ){    delete[] p; p = NULL;   }

/**
 * 安全に実装を隠蔽したオブジェクトのdeleteを行う
 * 実際には(Class*)にキャストされる。
 */
#define     IMPL_SAFE_DELETE( voidPtr, Class )  if(voidPtr){ delete ((Class*)(voidPtr)); voidPtr = NULL; }

/**
 * 一つでもフラグビットを保持していればtrueになる
 */
#define has_flag( flags, bit)       ( (flags & bit) != 0 )

/**
 * すべてのフラグビットを保持していればtrueになる
 */
#define has_flag_all(flags, bits)   ((flags& bits) == bits)

namespace jc {

//! プラットフォーム間依存を無くすために、極力プリミティブは代替型を利用する
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
/**
 * 固定小数値
 */
typedef signed int fixed32;
typedef signed long long s64;

/**
 * 固定小数値
 */
typedef signed long long fixed64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

/**
 * ポインタを強制キャストする場合に利用する
 */
typedef int int_pointer;

/**
 * CPU最適化した符号あり整数
 */
typedef int sint_cpu;

/**
 * CPU最適化した符号なし整数
 */
typedef unsigned int uint_cpu;

/**
 * 文字型
 */
typedef char charactor;

/**
 * 共通の真偽型
 */
typedef unsigned char jcboolean;

/**
 * 真
 * デフォルトは1、ただしjcfalse以外はすべてjctrueとして扱う
 */
const jcboolean jctrue = 1;

/**
 * 偽
 */
const jcboolean jcfalse = 0;

/**
 * trueかどうかを検証する
 */
inline jcboolean isTrue(const jcboolean b) {
    return b != jcfalse;
}

/**
 * falseかどうかを検証する
 */
inline jcboolean isFalse(const jcboolean b) {
    return b == jcfalse;
}

template<typename T>
inline T abs(const T v) {
    return v < 0 ? -v : v;
}

inline u32 abs(const u32 v) {
    return v;
}

inline u8 abs(const u8 v) {
    return v;
}

/**
 * 2つの浮動小数の同一性をそれなりにチェックする。
 * 演算誤差をある程度考慮する。
 */
inline jcboolean equals(const float a, const float b, const float _check) {
    return abs<float>(a - b) < _check;
}

/**
 * 2つの浮動小数の同一性をそれなりにチェックする。
 * 演算誤差をある程度考慮する。
 */
inline jcboolean equals(const float a, const float b) {
    return abs<float>(a - b) < 0.000001f;
}

/**
 * 整数演算の同一性チェック
 */
inline jcboolean equals(const s32 a, const s32 b) {
    return a == b;
}

/**
 * ２つの変数の値を交換する
 */
template<typename T>
inline void swap(T *a, T *b) {
    T temp = *a;
    *a = *b;
    *b = temp;
}

}

#endif /* JCMACRO_H_ */
