/*
 * EGLSupport.hpp
 *
 *  Created on: 2013/05/08
 */

#ifndef EGLERROR_HPP_
#define EGLERROR_HPP_

#include    "EGL/egl.h"

namespace ndk {

class EGLError {
    EGLError();
public:
    /**
     * エラーを出力する
     * GL_NOERROR以外だったらjctrueを返す
     */
    static jcboolean printEGLError(const charactor* file, const s32 line, GLenum error) {
        if (error == EGL_SUCCESS) {
            return jcfalse;
        }
#define LOG_EGL( error_enum )    case error_enum: ::jc::__logDebugF(error_enum != EGL_SUCCESS ? LogType_Alert : LogType_Debug, ::jc::__getFileName(file), "L %d | %s", line, #error_enum); return error != EGL_SUCCESS ? jctrue : jcfalse;
        switch (error) {
            LOG_EGL(EGL_NOT_INITIALIZED);
            LOG_EGL(EGL_BAD_ACCESS);
            LOG_EGL(EGL_BAD_ALLOC);
            LOG_EGL(EGL_BAD_ATTRIBUTE);
            LOG_EGL(EGL_BAD_CONFIG);
            LOG_EGL(EGL_BAD_CONTEXT);
            LOG_EGL(EGL_BAD_CURRENT_SURFACE);
            LOG_EGL(EGL_BAD_DISPLAY);
            LOG_EGL(EGL_BAD_MATCH);
            LOG_EGL(EGL_BAD_NATIVE_PIXMAP);
            LOG_EGL(EGL_BAD_NATIVE_WINDOW);
            LOG_EGL(EGL_BAD_PARAMETER);
            LOG_EGL(EGL_BAD_SURFACE);
            LOG_EGL(EGL_CONTEXT_LOST);
        }

        jclogf("EGL unknown error = 0x%x", error);
        return jctrue;
#undef LOG_EGL
    }

    /**
     * GLのエラー出力を行う
     */
    static jcboolean printEGLError(const charactor* file, const s32 line) {
        return printEGLError(file, line, eglGetError());
    }
};

#if defined(DEBUG) && !defined(NO_EGL_ASSERT)
#define     assert_egl(...)    { assert(::ndk::EGLError::printEGLError(__FILE__, __LINE__) == jcfalse); }
#else
// release
#define     assert_egl(...)    { }
#endif


}

#endif /* EGLERROR_HPP_ */
