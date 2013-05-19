/*
 * GLShader.cpp
 *
 *  Created on: 2012/06/27
 *      Author: Takeshi
 */

#include    "jointcoding.h"
#include    "jc/system/Exception.h"
#include    "jc/platform/Platform.h"
#include    "jc/gl/context/State.h"
#include    "jc/gl/shader/Shader.h"

namespace jc {
namespace gl {

Shader::Shader(const ShaderType_e type, const vram_handle &shader_handle) {
    this->type = type;
    this->shader = shader_handle;
}

Shader::~Shader() {
    this->dispose();
}

void Shader::dispose() {
    shader.reset();
}

/**
 * シェーダーのコンパイルを行う
 */
MGLShader Shader::compileFromUri(const ShaderType_e type, const VRAM vram, const Uri &uri) {
    MInputStream is = Platform::getFileSystem()->openInputStream(uri);
    MGLShader result;
    if (!is) {
        return result;
    }

    try {
        String fileText = InputStream::toText(is);
        jclog("------ shader source ------");
        jclogf("%s", fileText.c_str());
        jclog("------ shader source ------");
        return compile(type, vram, fileText.c_str());
    } catch (const Exception &e) {
        jcloge(e);
    }
    return result;
}

/**
 * シェーダーの作成を行う。
 */
MGLShader Shader::compile(const ShaderType_e type, const VRAM vram, const charactor* sourceCode) {
    // シェーダオブジェクトを作成
    vram_handle shader = vram->alloc(type == ShaderType_Vertex ? VRAM_VertexShader : VRAM_FragmentShader);
    assert(shader.get());

    // シェーダソースを設定
    const s32 src_length = strlen(sourceCode);
    jclogf("source chars = %d", src_length);
    glShaderSource(shader.get(), 1, &sourceCode, &src_length);
    jclog("check");
    // コンパイル
    glCompileShader(shader.get());
    jclog("check");
    assert_gl();
    jclog("check");

    // エラーチェック
    if (GLState::printShaderError(shader.get(), GL_COMPILE_STATUS)) {
        shader.reset();
        jclogf("shader(%s) comple error!!", type == ShaderType_Fragment ? "FRAGMENT" : "VERTEX");
        return MGLShader();
    }

    jclog("check");
    // エラーが発生していないから、オブジェクトを生成して返す
    Shader *result = new Shader(type, shader);
    jclog("check");
    return MGLShader(result);
}

}
}

