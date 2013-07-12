/*
 * JCGLIndexBufferObject.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCGLINDEXBUFFEROBJECT_H_
#define JCGLINDEXBUFFEROBJECT_H_

#include    "jointcoding.h"
#include    "jc/gl/gpu/VRAM.h"
#include    "jc/gl/gpu/Device.h"

namespace jc {
namespace gl {

/**
 * インデックスバッファを管理する
 *
 * 生成は内部で行うが、廃棄はVRAMクラスのgcに任せる。
 * Context間を移動する場合は必ずunbindを行い、ステートの残骸を残さないよう注意すること
 */
class IndexBufferObject: public Object {
    /**
     * 確保したインデックスバッファ
     */
    vram_handle indices;

    /**
     * インデックスバッファの配列長
     */
    u32 indices_length;

public:
    IndexBufferObject(MDevice device) {
        assert(device);

        indices = device->getVRAM()->alloc(VRAM_Indices);
        this->indices_length = 0;
    }

    virtual ~IndexBufferObject() {
    }

    /**
     * GLへ関連付ける
     */
    virtual void bind(MGLState state) {
        state->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.get());
    }

    /**
     * GLとの関連付けを解除する
     */
    virtual void unbind(MGLState state) {
        if (state->isBindedBuffer(GL_ELEMENT_ARRAY_BUFFER, indices.get())) {
            state->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    /**
     * データを転送する
     * bind()を行なってから呼び出すこと。
     * @param data 転送元のデータ
     * @param size 転送するバイト数
     * @param suage GL_STATIC_DRAW | GL_STREAM_DRAW | GL_DYNAMIC_DRAW
     */
    virtual void bufferData(const u16 *indices, const u32 indices_length, const GLenum usage) {
        this->indices_length = indices_length;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_length * sizeof(u16), (GLvoid*) indices, usage);
        assert_gl();
    }

    /**
     * データの一部分を転送する
     *
     * @param offset_num オフセット数を指定する。sizeof(u16)が自動的に適用されるため、注意する
     */
    virtual void bufferSubData(const s32 offset_num, const u16 *indices, const u32 indices_length) {
        assert(offset_num >= 0);
        assert((offset_num + indices_length) <= this->indices_length);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset_num * sizeof(u16), indices_length * sizeof(u16), indices);
        assert_gl();
    }

    /**
     * 配列数を取得する。
     * byte単位ではなく個数のため、byte数に変換する場合は *= sizeof(u16)が必要
     */
    virtual u32 length() const {
        return indices_length;
    }

    /**
     * レンダリングを行う
     * @param mode レンダリングモードを指定する デフォルトはGL_TRIANGLES
     */
    virtual void rendering(const GLenum mode) {
        glDrawElements(mode, indices_length, GL_UNSIGNED_SHORT, NULL);
        assert_gl();
    }

    /**
     * 指定した範囲のレンダリングを行う
     * @param mode レンダリングモードを指定する デフォルトはGL_TRIANGLES
     * @param indices_header indices[indices_header]からレンダリングを開始する
     * @param indices_length 指定した数のインデックスバッファを描画する
     */
    virtual void rendering(const GLenum mode, const GLsizei indices_header, const GLsizei indices) {
        glDrawElements(mode, indices, GL_UNSIGNED_SHORT, (GLvoid*) (sizeof(u16) * indices_header));
    }
};

/**
 * managed
 */
typedef jc_sp<IndexBufferObject> MIndexBufferObject;

}
}

#endif /* JCGLINDEXBUFFEROBJECT_H_ */
