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
#include "jcMacro.h"

/**
 * ndkネームスペース内でjcを利用できるようにする
 */
namespace ndk {
using namespace jc;
}

// include jni file
#include	"jni.h"

#include    "ndkNDK.h"
#include	"ndkSmartJObject.h"
#include	"ndkJniWrapper.h"
#include    "ndkJniArray.h"
#include    "ndkJniException.h"

#endif /* JOINTCODING_ANDROID_H_ */
