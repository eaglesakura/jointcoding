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
    GLObject shader;

    /**
     * タイプ
     */
    ShaderType_e type;

    Shader(const ShaderType_e type, const GLObject &shader_handle);
public:
    virtual ~Shader();

    /**
     * シェーダーオブジェクトを取得する。
     */
    virtual GLuint getName() const {
        return shader.get();
    }

    /**
     *
     */
    virtual const GLObject& getObject() const {
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
    static jc_sp<Shader> compile(MDevice device, const ShaderType_e type, const charactor* sourceCode, const u32 sourceCodeLength = 0);

    /**
     * シェーダーのコンパイルを行う
     */
    static jc_sp<Shader> compileFromUri(MDevice device, const ShaderType_e type, const Uri &uri);
};

/**
 * managed
 */
typedef jc_sp<Shader> MGLShader;

}
}
#endif /* JCGLSHADER_H_ */
