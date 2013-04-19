/*
 * ndkDeviceUtil.h
 *
 *  Created on: 2012/09/10
 */

#ifndef NDKDEVICEUTIL_H_
#define NDKDEVICEUTIL_H_

#include    "jc/gl/gpu/Device.h"
#include    "jni.h"

namespace ndk {
using namespace jc;
using namespace jc::gl;
/**
 * デバイスの作成を行う
 */
class EGLDeviceUtil {
public:
    /**
     * 新たにEGLデバイスを作成する。
     * この時点でデバイスと画面は関連付けられていない。
     */
    static MDevice createInstance();

    /**
     * EGL管理クラスを取得する。
     * このインスタンスはシングルトン管理される。
     * EGL資源はAndroidアプリ内で1つしか利用できない（必要がない）ため。
     */
    static MEGLProtocol getEGLProtocol();
};

}

#endif /* NDKDEVICEUTIL_H_ */
