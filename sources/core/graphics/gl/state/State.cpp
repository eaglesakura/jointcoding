/*
 * GLState.cpp
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */
#include "JCGLState.h"
#include "jcMath.h"
#include "jcglGPUCapacity.h"

namespace jc {
namespace gl {

GLState::GLState() {
}

GLState::~GLState() {
}

/**
 * シェーダーに関するエラーを出力する。
 * エラーが発生していたらtrueを返す。
 */
jcboolean GLState::printShaderError(const GLuint shaderObject, const GLint status) {
    GLint error = 0;
    glGetShaderiv(shaderObject, status, &error);
    if (!error) {
        // エラーが発生した
        GLint infoLen = 0;
        // エラーメッセージを取得
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            jc_sa<charactor> message( new charactor[infoLen] );
            glGetShaderInfoLog(shaderObject, infoLen, NULL, message.get());
            jclogf("shader error :: %s", message.get());
        }
        return jctrue;
    }
    return jcfalse;
}

/**
 * シェーダープログラムに関するエラーを出力する
 * エラーが発生していたらtrueを返す
 */
jcboolean GLState::printProgramError(const GLuint programObject, const GLint status) {
    GLint error = 0;
    glGetProgramiv(programObject, status, &error);
    if (!error) {
        // エラーが発生した
        GLint infoLen = 0;
        // エラーメッセージを取得
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            jc_sa<charactor> message( new charactor[infoLen] );
            glGetProgramInfoLog(programObject, infoLen, NULL, message.get());
            jclogf("shader error :: %s", message.get());
        }
        return jctrue;
    }
    return jcfalse;
}

/**
 * 現在のコンテキストと同期を取る。
 * この呼出は非常に重くなると予想されるので、極力呼び出さないようにする
 */
void GLState::syncContext() {
    jclog("------------------------- sync OpenGL ES Context ----------------------");
    // 命令の同期待を行う
    glFinish();
    // 描画色を取得する
    {
        GLfixed colors[4] = { 0 };
        glGetIntegerv(GL_COLOR_CLEAR_VALUE, (GLint*) colors);
        this->clearContext.clearColor.RGBAf(fixed2float(colors[0]), fixed2float(colors[1]), fixed2float(colors[2]), fixed2float(colors[3]));
    }
    // viewportを取得する
    {
        GLint xywh[4] = { 0 };
        glGetIntegerv(GL_VIEWPORT, xywh);
        viewportContext.setXYWH(xywh[0], xywh[1], xywh[2], xywh[3]);
    }
    // enable情報を取得する
    {
        enableContext.depthtest = glIsEnabled(GL_DEPTH_TEST);
        enableContext.texture2d = glIsEnabled(GL_TEXTURE_2D);
    }
    // シザーテスト情報を取得する
    {
        scissorContext.enable = glIsEnabled(GL_SCISSOR_TEST);
        glGetIntegerv(GL_SCISSOR_BOX, (GLint*) &scissorContext.box);
    }
    // シェーダプログラム情報を取得する
    {
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &shaderProgramContext.usingProgram);
    }
    // バッファを問い合わせる
    {
        glGetIntegerv(GL_ARRAY_BUFFER, (GLint*) &bindBufferContext.buffers[0]);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER, (GLint*) &bindBufferContext.buffers[1]);
    }
    // Attrに設定されている情報を取得する
    {
        const u32 maxAttr = min<u32>(GPUCapacity::getMaxVertexAttributes(), MAX_VERTEX_ATTRIBUTE);
        for (int i = 0; i < maxAttr; ++i) {
            // ショートカット用
            VertexAttributePointerData *attr = &vertexAttrContext.buffers[i].pointerData;
            GLint temp = 0;

            {
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, (GLint*) &attr->type);
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, (GLint*) &temp);
                {
                    attr->normalized = temp;
                }
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, (GLint*) &attr->size);
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, (GLint*) &attr->stride);
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, (GLint*) &attr->ptr);
            }

            temp = 0;
            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &temp);
            vertexAttrContext.buffers[i].enable = (jcboolean) temp;
        }
    }
    print(__FILE__, __LINE__);
    jclog("------------------ sync complete!! OpenGL ES Context ------------------");
}

/**
 * 現在のステートをログ出力する
 */
void GLState::print(const charactor* file, const s32 line) const {
    jclog("-------------------------  OpenGL ES Context ----------------------");
    // 描画色を取得する
    {
        jclogf("glClearColor RGBA(%d, %d, %d, %d)", (s32) clearContext.clearColor.r(), (s32) clearContext.clearColor.g(), (s32) clearContext.clearColor.b(), (s32) clearContext.clearColor.a());
    }
    // viewportを取得する
    {
        jclogf("glViewport LTRB(%d, %d, %d, %d)", viewportContext.left, viewportContext.top, viewportContext.right, viewportContext.bottom);
    }
    // enable情報を取得する
    {
        jclog("---- glEnable");
        jclogf("glEnable %s = %s", "GL_DEPTH_TEST", enableContext.depthtest ? "Enable" : "Disable");
        jclogf("glEnable %s = %s", "GL_TEXTURE_2D", enableContext.texture2d ? "Enable" : "Disable");
        jclog("---- glEnable Complete");
    }
    // シェーダプログラム情報を取得する
    {
        jclogf("glUseProgram %d", shaderProgramContext.usingProgram);
    }
    // バッファを問い合わせる
    {
        jclogf("glBindBuffer(%s) %d", "GL_ARRAY_BUFFER", bindBufferContext.buffers[0]);
        jclogf("glBindBuffer(%s) %d", "GL_ELEMENT_ARRAY_BUFFER", bindBufferContext.buffers[1]);
    }
    // Attrに設定されている情報を取得する
    {
        const u32 maxAttr = min<u32>(GPUCapacity::getMaxVertexAttributes(), MAX_VERTEX_ATTRIBUTE);
        for (int i = 0; i < maxAttr; ++i) {
            // ショートカット用
            const VertexAttributePointerData *attr = &vertexAttrContext.buffers[i].pointerData;
            jclogf("glVertexAttribPointer(index = %d, size = %d, type = %d, normalized = %s, stride = %d, ptr = 0x%X ) = %s",
            //
            // インデックス
                    i,
                    // 属性データ出力
                    attr->size, attr->type, attr->normalized ? "true" : "false", attr->stride, (s32) attr->ptr,
                    /// enable出力
                    vertexAttrContext.buffers[i].enable ? "enable" : "disable");
        }
    }
    jclog("--------------------------------------------------------------------");
}

/**
 * GLがエラーを持っている場合出力して、それ以外は何もしない。
 */
void GLState::printGLHasError(const charactor* file, const s32 line) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printGLError(file, line, error);
    }
}

/**
 * GLのエラー出力を行う
 */
void GLState::printGLError(const charactor* file, const s32 line) {
    printGLError(file, line, glGetError());
}

/**
 * エラーを出力する
 * GL_NOERROR以外だったらjctrueを返す
 */
jcboolean GLState::printGLError(const charactor* file, const s32 line, GLenum error) {
#define LOG_GL( error_enum )    case error_enum: ::jc::__logDebugF(error_enum != GL_NO_ERROR ? LogType_Alert : LogType_Debug, ::jc::__getFileName(file), "L %d | %s", line, #error_enum); return error != GL_NO_ERROR ? jctrue : jcfalse;
    switch (error) {
        LOG_GL(GL_NO_ERROR);
        LOG_GL(GL_INVALID_ENUM);
        LOG_GL(GL_INVALID_VALUE);
        LOG_GL(GL_INVALID_OPERATION);
//        LOG_GL(GL_STACK_OVERFLOW);
//        LOG_GL(GL_STACK_UNDERFLOW);
        LOG_GL(GL_OUT_OF_MEMORY);
//        LOG_GL(GL_TABLE_TOO_LARGE);
//        LOG_GL(GL_INVALID_ENUM);
    }

    jclogf("GL unknown error = 0x%x", error);
    return jctrue;
}

/*
 * GLに保存されているエラーをクリアする
 */
void GLState::clearGLError() {
    glGetError();
}

}
}
