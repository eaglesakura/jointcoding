/*
 * GLShaderProgram.cpp
 *
 *  Created on: 2012/06/27
 *      Author: Takeshi
 */

#include    "jc/gl/shader/Shader.h"
#include    "jc/gl/shader/ShaderProgram.h"
#include    "jc/system/Exception.h"

namespace jc {
namespace gl {

ShaderProgram::ShaderProgram(const GLObject &program, const MGLShader vertexShader, const MGLShader fragmentShader) {
    this->program = program;
    glGetProgramiv(program.get(), GL_ACTIVE_ATTRIBUTES, (GLint*) &attributes);
    glGetProgramiv(program.get(), GL_ACTIVE_ATTRIBUTES, (GLint*) &uniforms);
    assert_gl();
    this->vertexShader = vertexShader;
    this->fragmentShader = fragmentShader;
}

ShaderProgram::~ShaderProgram() {
    this->dispose();
}

/**
 * GLと関連付ける
 */
void ShaderProgram::bind(MGLState state) {
    state->useProgram(program.get());
}

/**
 * GLと切り離す
 */
void ShaderProgram::unbind(MGLState state) {
    if (state->isUseProgram(program.get())) {
        state->useProgram(0);
    }
}

/**
 * 管理しているリソースの解放を行う
 */
void ShaderProgram::dispose() {
    program.release();
    vertexShader.reset();
    fragmentShader.reset();
}

/**
 * プログラム属性変数のロケーションを取得する
 */
GLint ShaderProgram::getAttribLocation(const charactor* name) const {
    return glGetAttribLocation(program.get(), name);
}

/**
 * uniform変数のロケーションを取得する
 */
GLint ShaderProgram::getUniformLocation(const charactor *name) const {
    return glGetUniformLocation(program.get(), name);
}

/**
 * URIを利用してテキストファイルからシェーダーを作成する
 */
MGLShaderProgram ShaderProgram::buildFromUri(MDevice device, const Uri &vertexShaderUri, const Uri &fragmentShaderUri) {
    MGLShader vertexShader; // ビルドした頂点シェーダ
    MGLShader fragmentShader; // ビルドしたフラグメントシェーダ
    MGLShaderProgram result;

    // 頂点シェーダを作成
    try {
        vertexShader = Shader::compileFromUri(device, ShaderType_Vertex, vertexShaderUri);
        if (!vertexShader) {
            return result;
        }
    } catch (const Exception &e) {
        jcloge(e);
        return result;
    }
    // フラグメントシェーダを作成
    try {
        fragmentShader = Shader::compileFromUri(device, ShaderType_Fragment, fragmentShaderUri);
        if (!fragmentShader) {
            return result;
        }
    } catch (const Exception &e) {
        jcloge(e);
        return result;
    }

    // リンクを行う
    return link(device, vertexShader, fragmentShader);
}

/**
 * ソースコードから直接ビルドを行う
 */
MGLShaderProgram ShaderProgram::buildFromSource(MDevice device, const charactor *vertex_shader_source, const charactor* fragment_shader_source) {
    MGLShader vertexShader; // ビルドした頂点シェーダ
    MGLShader fragmentShader; // ビルドしたフラグメントシェーダ
    MGLShaderProgram result;

    // 頂点シェーダを作成
    try {
        vertexShader = Shader::compile(device, ShaderType_Vertex, vertex_shader_source);
        if (!vertexShader) {
            return result;
        }
    } catch (const Exception &e) {
        jcloge(e);
        return result;
    }
    // フラグメントシェーダを作成
    try {
        fragmentShader = Shader::compile(device, ShaderType_Fragment, fragment_shader_source);
        if (!fragmentShader) {
            return result;
        }
    } catch (const Exception &e) {
        jcloge(e);
        return result;
    }

    // リンクを行う
    return link(device, vertexShader, fragmentShader);
}

/**
 * 実行用にシェーダーをリンクさせる。
 */ //
jc_sp<ShaderProgram> ShaderProgram::link(MDevice device, const MGLShader vertexShader, const MGLShader fragmentShader) {
    assert(vertexShader);
    assert(vertexShader->getType() == ShaderType_Vertex);
    assert(fragmentShader);
    assert(fragmentShader->getType() == ShaderType_Fragment);

    const MGLState state = device->getState();

    GLObject program;
    program.alloc(device->getVRAM(VRAM_ShaderProgram));
    assert(program);

    // 頂点シェーダを対応付ける
    glAttachShader(program.get(), vertexShader->getName());
    // フラグメントシェーダを対応付ける
    glAttachShader(program.get(), fragmentShader->getName());
    // シェーダをリンクさせる
    glLinkProgram(program.get());
    assert_gl();

    if (GLState::printProgramError(program.get(), GL_LINK_STATUS)) {
        // リンクに失敗したからエラーを返す
        program.release();
        return MGLShaderProgram();
    }

    // リンクに成功した
    ShaderProgram *result = new ShaderProgram(program, vertexShader, fragmentShader);
    return MGLShaderProgram(result);
}

}
}
