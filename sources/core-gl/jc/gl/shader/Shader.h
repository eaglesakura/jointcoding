/*
 * JCGLShader.h
 *
 *  Created on: 2012/06/27
 *      Author: Takeshi
 */

#ifndef JCGLSHADER_H_
#define JCGLSHADER_H_

#include    "jc/mem/SmartPtr.h"
#include    "jc/io/Uri.h"
#include    "jc/gl/GL.h"
#include    "jc/gl/gpu/Device.h"

namespace jc {
namespace gl {

enum ShaderType_e {
    /**
     * 頂点シェーダーを示す
     */
    ShaderType_Vertex = GL_VERTEX_SHADER,

    /**
     * フラグメントシェーダーを示す。
     */
    ShaderType_Fragment = GL_FRAGMENT_SHADER
}
;

/**
 * シェーダオブジェクトを管理する。
 */
class Shader: public Object {
protected:
    /**
     * ビルドしたシェーダー
     */
    vram_handle shader;

    /**
     * タイプ
     */
    ShaderType_e type;

    Shader(const ShaderType_e type, const vram_handle &shader_handle);
public:
    virtual ~Shader();

    /**
     * シェーダーオブジェクトを取得する。
     */
    virtual vram_handle& getShader() {
        return shader;
    }

    virtual ShaderType_e getType() {
        return type;
    }

    /**
     * 管理しているリソースを明示的に廃棄する
     */
    virtual void dispose();

    /**
     * シェーダーの作成を行う。
     */
    static jc_sp<Shader> compile(const ShaderType_e type, const VRAM vram, const charactor* sourceCode);

    /**
     * シェーダーのコンパイルを行う
     */
    static jc_sp<Shader> compileFromUri(const ShaderType_e type, const VRAM vram, const Uri &uri);
};

/**
 * managed
 */
typedef jc_sp<Shader> MGLShader;

}
}
#endif /* JCGLSHADER_H_ */
