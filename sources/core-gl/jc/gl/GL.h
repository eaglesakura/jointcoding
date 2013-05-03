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
#import     <OpenGLES/ES2/gl.h>
#import     <OpenGLES/ES2/glext.h>
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

#if defined(DEBUG) && !defined(NO_GL_ASSERT)
#define     assert_gl(...)    { assert(::jc::gl::GLState::printGLHasError(__FILE__, __LINE__) == jcfalse); }
#else
// release
#define     assert_gl(...)    { }
#endif

/**
 * 無効な属性インデックスを示す定数
 */
#define ATTRIBUTE_DISABLE_INDEX -1

/**
 * 無効な属性インデックスを示す
 */
#define UNIFORM_DISABLE_INDEX -1

}
}
#endif /* JCGL_H_ */
