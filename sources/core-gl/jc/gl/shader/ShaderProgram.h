/*
 * JCGLShaderProgram.h
 *
 *  Created on: 2012/06/27
 *      Author: Takeshi
 */

#ifndef JCGLSHADERPROGRAM_H_
#define JCGLSHADERPROGRAM_H_

#include    "jc/mem/SmartPtr.h"
#include    "jc/io/Uri.h"
#include    "jc/gl/gpu/Device.h"
#include    "jc/gl/shader/Shader.h"

#define JCGL_NOTFOUND_ATTR  -1

namespace jc {
namespace gl {

/**
 * リンクを行ったシェーダー本体
 */
class ShaderProgram: public Object {
protected:
    /**
     * ステート保持
     */
    MGLState state;

    /**
     * 関連付けられた頂点シェーダ
     */
    MGLShader vertexShader;

    /**
     * 関連付けられたフラグメントシェーダ
     */
    MGLShader fragmentShader;

    /**
     * シェーダに設定されているAttribute数
     */
    u32 attributes;

    /**
     * シェーダに設定されているuniform数
     */
    u32 uniforms;

    /**
     * Fragment/VertexShaderをまとめているシェーダプログラム
     */
    vram_handle program;

    ShaderProgram(const vram_handle &program, const MGLState state, const MGLShader vertexShader, const MGLShader fragmentShader);
public:
    virtual ~ShaderProgram();

    /**
     * 管理しているリソースの解放を行う
     */
    virtual void dispose();
//
//    /**
//     * プログラムオブジェクトを取得する。
//     */
//    virtual SharedResource& getProgram() {
//        return program;
//    }

    /**
     * プログラム属性変数のロケーションを取得する
     */
    virtual GLint getAttribLocation(const charactor* name) const;

    /**
     * uniform変数のロケーションを取得する
     */
    virtual GLint getUniformLocation(const charactor *name) const;

    /**
     * Attribute数を取得する
     */
    virtual u32 getMaxAttributes() const {
        return attributes;
    }

    /**
     * Uniform数を取得する
     */
    virtual u32 getMaxUniforms() const {
        return uniforms;
    }

    /**
     * GLと関連付ける
     */
    virtual void bind();

    /**
     * GLと切り離す
     */
    virtual void unbind();

    /**
     * URIを利用してテキストファイルからシェーダーを作成する
     */
    static jc_sp<ShaderProgram> buildFromUri(MDevice device, const Uri &vertexShaderUri, const Uri &fragmentShaderUri );

    /**
     * ソースコードから直接ビルドを行う
     */
    static jc_sp<ShaderProgram> buildFromSource(MDevice device, const charactor *vertex_shader_source, const charactor* fragment_shader_source);

    /**
     * 実行用にシェーダーをリンクさせる。
     * 失敗した場合、NULLを返す。
     */
    static jc_sp<ShaderProgram> link(MDevice device, const MGLShader vertexShader, const MGLShader fragmentShader);
};

/**
 * managed
 */
typedef jc_sp< ShaderProgram> MGLShaderProgram;

}
}
#endif /* JCGLSHADERPROGRAM_H_ */
