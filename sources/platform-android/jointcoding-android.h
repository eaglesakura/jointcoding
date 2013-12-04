/*
 * jointcoding-android.h
 *
 *  Created on: 2012/05/12
 *      Author: Takeshi
 */

#ifndef JOINTCODING_ANDROID_H_
#define JOINTCODING_ANDROID_H_

/**
 * Android版のビルドを示す
 */
#include "jointcoding.h"
#include "jc/system/Macro.h"
#include    "jc/collection/SafeArray.hpp"

namespace ndk {
using namespace jc;
}

// include jni file
#include	<jni.h>

#include    "jcandroid/ndk-support.h"
#include	"jcandroid/jni/SmartJObject.h"
#include	"jcandroid/jni/JniWrapper.h"
#include    "jcandroid/jni/JniArray.h"
#include    "android-classes/ndkJniException.h"
#include    "jcandroid/util/Utils.h"
#include    "jcandroid/jni/Jointable.h"

#endif /* JOINTCODING_ANDROID_H_ */
