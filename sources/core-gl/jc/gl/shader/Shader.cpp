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

Shader::Shader(const ShaderType_e type, const GLObject &shader_handle) {
    this->type = type;
    this->shader = shader_handle;
}

Shader::~Shader() {
    this->dispose();
}

void Shader::dispose() {
    shader.release();
}

/**
 * シェーダーのコンパイルを行う
 */
MGLShader Shader::compileFromUri(MDevice device, const ShaderType_e type, const Uri &uri) {
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
        return compile(device, type, fileText.c_str());
    } catch (const Exception &e) {
        jcloge(e);
    }
    return result;
}

/**
 * シェーダーの作成を行う。
 */
MGLShader Shader::compile(MDevice device, const ShaderType_e type, const charactor* sourceCode) {
    // シェーダオブジェクトを作成
    GLObject shader;
    shader.alloc(device->getVRAM(type == ShaderType_Vertex ? VRAM_VertexShader : VRAM_FragmentShader));
    assert(shader.get());

    // シェーダソースを設定
    const s32 src_length = strlen(sourceCode);
    jclogf("source chars = %d", src_length);
    glShaderSource(shader.get(), 1, &sourceCode, &src_length);
    // コンパイル
    glCompileShader(shader.get());
    assert_gl();

    // エラーチェック
    if (GLState::printShaderError(shader.get(), GL_COMPILE_STATUS)) {
        shader.release();
        jclogf("shader(%s) comple error!!", type == ShaderType_Fragment ? "FRAGMENT" : "VERTEX");
        return MGLShader();
    }

    // エラーが発生していないから、オブジェクトを生成して返す
    Shader *result = mark_new Shader(type, shader);
    return MGLShader(result);
}

}
}

