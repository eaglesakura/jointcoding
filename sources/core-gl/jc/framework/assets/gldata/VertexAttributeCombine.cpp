/*
 * VertexAttributeCombine.cpp
 *
 *  Created on: 2013/07/11
 */

#include    "jc/framework/assets/GLAsset.hpp"

namespace jc {
namespace fw {

namespace {
static const struct {
    /**
     * データサイズ
     */
    short bytes;

    /**
     * データ数
     */
    short datanum;

    /**
     * normalized
     */
    GLenum normalized;

    /**
     * data type
     */
    GLenum datatype;
} g_attributes[] = {
/**
 * VertexAttributeData_float1
 */
{ sizeof(GLfloat), 1, GL_FALSE, GL_FLOAT },

/**
 * VertexAttributeData_float2
 */
{ sizeof(GLfloat), 2, GL_FALSE, GL_FLOAT },

/**
 * VertexAttributeData_float3
 */
{ sizeof(GLfloat), 3, GL_FALSE, GL_FLOAT },

/**
 * VertexAttributeData_float4
 */
{ sizeof(GLfloat), 4, GL_FALSE, GL_FLOAT },

/**
 * VertexAttributeData_ubyte4
 */
{ sizeof(GLubyte), 4, GL_FALSE, GL_UNSIGNED_BYTE },

/**
 * VertexAttributeData_ubyte4_normalized
 */
{ sizeof(GLubyte), 4, GL_TRUE, GL_UNSIGNED_BYTE },

/**
 * VertexAttributeData_ushort1_normalized
 */
{ sizeof(GLushort), 1, GL_TRUE, GL_UNSIGNED_SHORT },

{ sizeof(GLint), 1, GL_FALSE, GL_INT },

};

}

VertexAttributeCombine::VertexAttributeCombine() {
    enables.allEnable();
    vertexStructSize = 0;

}

VertexAttributeCombine::~VertexAttributeCombine() {

}

/**
 * 属性情報を組み立てる
 *
 * 頂点構造体の配置とVertexAttributeRequestのインデックスはリンクさせる必要がある
 */
void VertexAttributeCombine::request(MGLShaderProgram shader, const VertexAttributeRequest *requests, const int requests_length) {
    // キャッシュを生成する
    attributeCache.alloc(requests_length);

    vertexStructSize = 0;
    // リクエスト数だけ処理を行う
    for (int i = 0; i < requests_length; ++i) {
        attributeCache[i].location = shader->getAttribLocation(requests[i].name);

#ifdef  DEBUG
        if (requests[i].valid) {
            assert(attributeCache[i].location >= 0);
        }
#endif

        attributeCache[i].data_type = requests[i].type;
        attributeCache[i].offset_header = vertexStructSize;

        // オフセットを加算する
        vertexStructSize += (g_attributes[requests[i].type].bytes * g_attributes[requests[i].type].datanum);
    }
}

/**
 * ステートに関連付ける
 * シェーダーはバインド済みである必要がある
 */
void VertexAttributeCombine::attributePointer(MGLState state, void* vertex_ptr) {

    for (int i = 0; i < attributeCache.length; ++i) {
        // ロケーションが有効であり、属性利用も有効である場合は属性設定する
        if (attributeCache[i].location != ATTRIBUTE_DISABLE_INDEX && enables.isEnable(i)) {

#define __location              attributeCache[i].location
#define __offset                attributeCache[i].offset_header
#define __attribute_datatype    g_attributes[attributeCache[i].data_type].datatype
#define __attribute_datanum    g_attributes[attributeCache[i].data_type].datanum
#define __attribute_normalized    g_attributes[attributeCache[i].data_type].normalized

            state->enableVertexAttribArray(__location);
            state->vertexAttribPointer(__location,__attribute_datanum, __attribute_datatype, __attribute_normalized,vertexStructSize, vertex_ptr, __offset);

        }
    }
}

}
}

#undef  __location
#undef  __offset
#undef  __attribute_datatype
#undef  __attribute_datanum
#undef  __attribute_normalized
