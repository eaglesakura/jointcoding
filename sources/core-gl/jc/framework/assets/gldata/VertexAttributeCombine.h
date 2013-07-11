/*
 * VertexAttributeCombine.h
 *
 *  Created on: 2013/07/10
 */

#ifndef VERTEXATTRIBUTECOMBINE_H_
#define VERTEXATTRIBUTECOMBINE_H_

#include    "jc/framework/assets/GLAsset.hpp"

namespace jc {
namespace fw {

/**
 * 頂点属性の設定を行う
 * アラインメントチェックを行わないため、バイト境界に注意する
 */
enum VertexAttributeData_e {
    /**
     * GLfloat[1]
     * GL_FLOAT
     * normalized=false
     */
    VertexAttributeData_float1,

    /**
     * GLfloat[2]
     * GL_FLOAT
     * normalized=false
     */
    VertexAttributeData_float2,

    /**
     * GLfloat[3]
     * GL_FLOAT
     * normalized=false
     */
    VertexAttributeData_float3,

    /**
     * GLfloat[4]
     * GL_FLOAT
     * normalized=false
     */
    VertexAttributeData_float4,

    /**
     * GLubyte[4]
     * GL_UNSIGNED_BYTE
     * normalized=true
     */
    VertexAttributeData_ubyte4_normalized,

    /**
     * GLushort
     * GL_UNSIGNED_SHORT
     * normalized=true
     */
    VertexAttributeData_ushort_normalized,
};

/**
 * 属性情報配列を組み立てるためのリクエスト情報
 */
struct VertexAttributeRequest {
    /**
     * 属性名
     */
    charactor *name;

    /**
     * 属性タイプ
     */
    VertexAttributeData_e type;
};

/**
 * 属性情報配列からキャッシュを生成する
 */
struct VertexAttributeCache {
    /**
     * シェーダーロケーション
     */
    GLint location;

    /**
     * 頂点配列のヘッダ（0byte目）からのオフセット値
     */
    GLuint offset_header;

    /**
     * 属性設定
     */
    VertexAttributeData_e data_type;
};

/**
 * 頂点属性の組み合わせを設定する
 */
class VertexAttributeComtine: public Object {

    /**
     * 属性キャッシュ
     */
    safe_array<VertexAttributeCache> attributeCache;

    /**
     * 頂点構造体のサイズ
     */
    s32 vertexStructSize;

    /**
     * 有効な頂点属性の組み合わせを管理する
     */
    SimpleBitFlags enables;
public:
    VertexAttributeComtine();

    virtual ~VertexAttributeComtine();

    /**
     *
     */
    virtual void setEnableAttributes(const SimpleBitFlags enables) {
        this->enables = enables;
    }

    /**
     * 属性情報を組み立てる
     *
     * 頂点構造体の配置とVertexAttributeRequestのインデックスはリンクさせる必要がある
     */
    virtual void request(MGLShaderProgram shader, const VertexAttributeRequest *requests, const int requests_length);

    /**
     * ステートに関連付ける
     * シェーダーはバインド済みである必要がある
     */
    virtual void attribute(MGLState state, void* vertex_ptr = NULL);
};

}
}

#endif /* VERTEXATTRIBUTECOMBINE_H_ */
