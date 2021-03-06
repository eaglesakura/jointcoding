/*
 * JCGL.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCGL_H_
#define JCGL_H_

#include "jointcoding.h"

// include gl
#ifdef BUILD_MacOSX
#include    <GL/gl3.h>

#else

#ifdef BUILD_Android
#include    "GLES2/gl2.h"
#include    "GLES2/gl2ext.h"
#endif // BUILD_Android

#ifdef BUILD_iOS
#include <OpenGLES/ES2/gl.h>
#endif // BUILD_iOS

#endif

namespace jc {

namespace gl {

/**
 * GL用の固定小数1.0fを示す
 */
const s32 /* GLfixed */FIXED_ONE = 0x10000;

/**
 * テクスチャのNULLオブジェクトを示す。
 */
const GLuint TEXTURE_NULL = (GLuint) 0;

/**
 * バッファのNULLオブジェクトを示す。
 */
const GLuint BUFFER_NULL = (GLuint) 0;

/**
 * GL用の固定小数をfloatへ変換する
 */
inline float fixed2float(s32 /* GLfixed */fixed) {
    return (float) (fixed) / (float) FIXED_ONE;
}

/**
 * float値をGL用の固定小数へ変換する
 */
inline s32 /* GLfixed */float2fixed(float f) {
    return (s32) (f * FIXED_ONE);
}

}
}
#endif /* JCGL_H_ */
