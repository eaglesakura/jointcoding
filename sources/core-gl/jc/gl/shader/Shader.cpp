/*
 * GLShader.cpp
 *
 *  Created on: 2012/06/27
 *      Author: Takeshi
 */

#include    "jc/gl/Shader.h"
#include    "jc/system/Exception.h"
#include    "jc/platform/Platform.h"
#include    "jc/gl/State.h"

namespace jc {
namespace gl {

Shader::Shader(const SharedResource &shaderResource) {
    jclogf("vram2vram = %x", shaderResource.get());
    this->shader = shaderResource;
    jclogf("vram2vram finish = %x", shaderResource.get());
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
MGLShader Shader::compileFromUri(const ShaderType_e type, const VRAM vram, const Uri &uri) {
    MInputStream is = Platform::getFileSystem()->openInputStream(uri);
    MGLShader result;
    if (!is) {
        return result;
    }

    try {
        String fileText = InputStream::toText(is);
//        return result;
        jclog("------ shader source ------");
        jclogf("%s", fileText.c_str());
        jclog("------ shader source ------");
        /*
         * */
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
    CLEAR_GL_ERROR
    ;
    // シェーダオブジェクトを作成
    SharedResource shader;
    shader.alloc(vram, type == ShaderType_Vertex ? VRAM_VertexShader : VRAM_FragmentShader);

    // シェーダソースを設定
    const s32 src_length = strlen(sourceCode);
    jclogf("source chars = %d", src_length);
    glShaderSource(shader.get(), 1, &sourceCode, &src_length);
    // コンパイル
    glCompileShader(shader.get());
    // エラーチェック
    if (GLState::printShaderError(shader.get(), GL_COMPILE_STATUS)) {
        shader.release();
        jclogf("shader(%d) comple error!!", type);
        return MGLShader();
    }

    // エラーが発生していないから、オブジェクトを生成して返す
    Shader *result = new Shader(shader);
    return MGLShader(result);
}

}
}

