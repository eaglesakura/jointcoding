/*
 * GLState.cpp
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#include    "jointcoding.h"
#include    "jc/math/Math.h"
#include    "jc/gl/gpu/GPUCapacity.h"
#include    "jc/gl/context/State.h"
#include    "jc/mem/MemoryUtil.h"

namespace jc {
namespace gl {

GLState::GLState() {
    // GPUステータスを取り出す
    GPUCapacity::initialize();
    {
        lineWidthContext = 0.0f;
    }
    // clear color
    {
        clearContext.clearColor = Color::fromRGBAi(0, 0, 0, 0);
    }
    // attribute
    {
        for (int i = 0; i < MAX_VERTEX_ATTRIBUTE; ++i) {
            vertexAttrContext.buffers[i].enable = jcfalse;
            zeromemory((void*) &vertexAttrContext.buffers[i].pointerData, sizeof(VertexAttributePointerData));
        }
    }
    // blend
    {
        blendContext.enable = jcfalse;
        blendContext.src = GL_ONE;
        blendContext.dst = GL_ZERO;
    }
    // depth
    {
        depthContext.enable = jcfalse;
        depthContext.func = GL_LEQUAL;
    }
    // scissor
    {
        scissorContext.box.setXYWH(0, 0, 0, 0);
        scissorContext.enable = jcfalse;
    }
    // viewport
    {
        viewportContext.setXYWH(0, 0, 0, 0);
    }
    // shader
    {
        shaderProgramContext.usingProgram = 0;
    }
    // reset texture params
    {
        textureContext.active = jcfalse;
        for (int i = 0; i < MAX_TEXTURE_UNIT; ++i) {
            textureContext.textures[i] = 0;
            textureContext.targets[i] = 0;
        }
    }
    // reset buffer params
    {
        for (int i = 0; i < MAX_BINDBUFFER; ++i) {
            bindBufferContext.buffers[i] = 0;
        }
    }
    // caps
    {
        caps.MAX_TEXTURE_UNITS = GPUCapacity::getMaxTextureUnits();
    }
    // mask
    {
        maskContext.r = maskContext.g = maskContext.b = maskContext.a = jctrue;
    }
    // frameBuffer
    {
        frameBufferContext.frameBuffer = 0;
        frameBufferContext.renderBuffer = 0;
    }
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
            jc_sa<charactor> message(mark_new charactor[infoLen]);
            glGetShaderInfoLog(shaderObject, infoLen, NULL, message.get());
            jclogf("shader error :: %s", message.get());
        } else {
            jclog("comple error not info...");
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
            jc_sa<charactor> message(mark_new charactor[infoLen]);
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
    finish();
    // 描画色を取得する
    {
        s32 colors[4] = { 0 };
        glGetIntegerv(GL_COLOR_CLEAR_VALUE, (GLint*) colors);
        assert_gl();
        this->clearContext.clearColor.RGBAf(fixed2float(colors[0]), fixed2float(colors[1]), fixed2float(colors[2]), fixed2float(colors[3]));
    }
    // viewportを取得する
    {
        GLint xywh[4] = { 0 };
        glGetIntegerv(GL_VIEWPORT, xywh);
        assert_gl();
        viewportContext.setXYWH(xywh[0], xywh[1], xywh[2], xywh[3]);
    }
    // blend情報を取得する
    {
        blendContext.enable = glIsEnabled(GL_BLEND);
        assert_gl();
        blendContext.src = blendContext.srcAlpha = blendContext.dst = blendContext.dstAlpha = 0;
//        glGetIntegerv(GL_BLEND_SRC_RGB, &blendContext.src);
//        glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendContext.srcAlpha);
//        glGetIntegerv(GL_BLEND_DST_RGB, &blendContext.dst);
//        glGetIntegerv(GL_BLEND_DST_ALPHA, &blendContext.dstAlpha);
    }
    // depth
    {
        depthContext.enable = glIsEnabled(GL_DEPTH_TEST);
        assert_gl();
        glGetIntegerv(GL_DEPTH_FUNC, (GLint*) &depthContext.func);
        assert_gl();
    }
    // シザーテスト情報を取得する
    {
        scissorContext.enable = glIsEnabled(GL_SCISSOR_TEST);
        assert_gl();
        glGetIntegerv(GL_SCISSOR_BOX, (GLint*) &scissorContext.box);
        assert_gl();
    }
    // シェーダプログラム情報を取得する
    {
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &shaderProgramContext.usingProgram);
        assert_gl();
    }
    // バッファを問い合わせる
    {
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*) &bindBufferContext.buffers[0]);
        assert_gl();
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*) &bindBufferContext.buffers[1]);
        assert_gl();
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*) &frameBufferContext.frameBuffer);
        assert_gl();
        glGetIntegerv(GL_RENDERBUFFER_BINDING, (GLint*) &frameBufferContext.renderBuffer);
        assert_gl();
    }
    // マスクを問い合わせる
    {
        {
            GLboolean colors[4];
            glGetBooleanv(GL_COLOR_WRITEMASK, colors);
            assert_gl();

            maskContext.r = colors[0];
            maskContext.g = colors[1];
            maskContext.b = colors[2];
            maskContext.a = colors[3];
        }
    }
    // Attrに設定されている情報を取得する
    {
        const u32 maxAttr = min<u32>(GPUCapacity::getMaxVertexAttributes(), MAX_VERTEX_ATTRIBUTE);
        for (u32 i = 0; i < maxAttr; ++i) {
            // ショートカット用
            VertexAttributePointerData *attr = &vertexAttrContext.buffers[i].pointerData;
            GLint temp = 0;

            {
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, (GLint*) &attr->type);
                assert_gl();
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, (GLint*) &temp);
                assert_gl();
                {
                    attr->normalized = temp;
                }
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_SIZE, (GLint*) &attr->size);
                assert_gl();
                glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, (GLint*) &attr->stride);
                assert_gl();
                glGetVertexAttribPointerv(i, GL_VERTEX_ATTRIB_ARRAY_POINTER, (GLvoid**) &attr->ptr);
                assert_gl();
            }

            temp = 0;
            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &temp);
            assert_gl();
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
        jclogf("glClearColor RGBA(%d, %d, %d, %d)", (s32 ) clearContext.clearColor.r(), (s32 ) clearContext.clearColor.g(), (s32 ) clearContext.clearColor.b(), (s32 ) clearContext.clearColor.a());
    }
    // viewportを取得する
    {
        jclogf("glViewport LTRB(%d, %d, %d, %d)", viewportContext.left, viewportContext.top, viewportContext.right, viewportContext.bottom);
    }
    // depth
    {
        jclog("---- glDepth");
        jclogf("glEnable %s = %s", "GL_DEPTH_TEST", depthContext.enable ? "Enable" : "Disable");
        jclogf("glDepthFunc(%x)", depthContext.func);
        jclog("---- glDepth Complete");
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
        for (u32 i = 0; i < maxAttr; ++i) {
            // ショートカット用
            const VertexAttributePointerData *attr = &vertexAttrContext.buffers[i].pointerData;
            jclogf("glVertexAttribPointer(index = %d, size = %d, type = 0x%x, normalized = %s, stride = %d, ptr = 0x%X ) = %s",
            //
            // インデックス
                    i,
                    // 属性データ出力
                    attr->size, attr->type, attr->normalized ? "true" : "false", attr->stride, attr->ptr,
                    /// enable出力
                    vertexAttrContext.buffers[i].enable ? "enable" : "disable");
        }
    }
    jclog("--------------------------------------------------------------------");
}

/**
 * GLがエラーを持っている場合出力して、それ以外は何もしない。
 */
jcboolean GLState::printGLHasError(const charactor* file, const s32 line) {
#ifdef DEBUG
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        return printGLError(file, line, error);
    }
#endif
    return jcfalse;
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
        LOG_GL(GL_INVALID_ENUM)
            ;
        LOG_GL(GL_INVALID_VALUE)
            ;
        LOG_GL(GL_INVALID_OPERATION)
            ;
        LOG_GL(GL_OUT_OF_MEMORY)
            ;
        LOG_GL(GL_NO_ERROR)
            ;
//        LOG_GL(GL_STACK_OVERFLOW);
//        LOG_GL(GL_STACK_UNDERFLOW);
//        LOG_GL(GL_TABLE_TOO_LARGE);
    }

    jclogf("GL unknown error = 0x%x", error);
    return jctrue;
#undef  LOG_GL
}

/**
 * デフォルト設定に従ってブレンドを行う
 */
jcboolean GLState::blendFunc(const GLBlendType_e type) {
    static const GLenum funcTable[][4] = {
    //      sfactor  dfactor sfactorA dfactorA
    //      alpha
            { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE },
            // add
            { GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE },
            // over
            { GL_ZERO, GL_ZERO, GL_ONE, GL_ZERO }, };
    return blendFunc(funcTable[type][0], funcTable[type][1], funcTable[type][2], funcTable[type][3]);
//    glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX_EXT);
//    glBlendFuncSeparate(sfactor[type], dfactor[type], sfactorAlpha[type], dfactorAlpha[type]);
//    return jctrue;
//    return blendFunc(sfactor[0], dfactor[0], sfactorAlpha[0], dfactorAlpha[0]);
}

/*
 * GLに保存されているエラーをクリアする
 */
void GLState::clearGLError() {
    glGetError();
}

}
}
