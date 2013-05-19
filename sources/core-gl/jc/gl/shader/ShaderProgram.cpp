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

ShaderProgram::ShaderProgram(const vram_handle &program, const MGLState state, const MGLShader vertexShader, const MGLShader fragmentShader) {
    this->program = program;
    this->state = state;
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
void ShaderProgram::bind() {
    state->useProgram(program.get());
}

/**
 * GLと切り離す
 */
void ShaderProgram::unbind() {
    if (state->isUseProgram(program.get())) {
        state->useProgram(0);
    }
}

/**
 * 管理しているリソースの解放を行う
 */
void ShaderProgram::dispose() {
    if (program.exist()) {
        unbind();
        // 各々のシェーダーのデタッチと削除を行う
        program.reset();
    }

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
    return  glGetUniformLocation(program.get(), name);
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
        vertexShader = Shader::compileFromUri(ShaderType_Vertex, device->getVRAM(), vertexShaderUri);
        if (!vertexShader) {
            return result;
        }
    } catch (const Exception &e) {
        jcloge(e);
        return result;
    }
    // フラグメントシェーダを作成
    try {
        fragmentShader = Shader::compileFromUri(ShaderType_Fragment, device->getVRAM(), fragmentShaderUri);
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
        vertexShader = Shader::compile(ShaderType_Vertex, device->getVRAM(), vertex_shader_source);
        if (!vertexShader) {
            return result;
        }
    } catch (const Exception &e) {
        jcloge(e);
        return result;
    }
    // フラグメントシェーダを作成
    try {
        fragmentShader = Shader::compile(ShaderType_Fragment, device->getVRAM(), fragment_shader_source);
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

    vram_handle program = device->getVRAM()->alloc(VRAM_ShaderProgram);

    // 頂点シェーダを対応付ける
    glAttachShader(program.get(), vertexShader->getShader().get());
    // フラグメントシェーダを対応付ける
    glAttachShader(program.get(), fragmentShader->getShader().get());
    // シェーダをリンクさせる
    glLinkProgram(program.get());
    assert_gl();

    if (GLState::printProgramError(program.get(), GL_LINK_STATUS)) {
        // リンクに失敗したからエラーを返す
        program.reset();
        return MGLShaderProgram();
    }

    // リンクに成功した
    ShaderProgram *result = new ShaderProgram(program, device->getState(), vertexShader, fragmentShader);
    return MGLShaderProgram(result);
}

}
}
