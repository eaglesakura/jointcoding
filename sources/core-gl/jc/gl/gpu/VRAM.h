/*
 * jcGLVRAM.h
 *
 *  Created on: 2012/08/22
 */

#ifndef JCGLVRAM_H_
#define JCGLVRAM_H_

#include    "jc/system/Macro.h"
#include    "jc/mem/SmartPtr.h"
#include    "jc/collection/ArrayHandle.hpp"
#include    "jc/system/StlAllocator.hpp"
#include    <vector>
#include    <list>
#include    "jc/gl/GL.h"

namespace jc {
namespace gl {

/**
 * 資源の種類を示す。
 *
 * @note 順番を変えると関数テーブルとの整合性がとれなくなるから、絶対に順番を変えないこと。
 */
enum VRAM_e {
    /**
     * テクスチャ資源を示す
     */
    VRAM_Texture,

    /**
     * Indices資源を示す
     */
    VRAM_Indices,

    /**
     * VBO資源を示す
     */
    VRAM_VertexBufferObject,

    /**
     * Vertexシェーダー資源を示す
     */
    VRAM_VertexShader,

    /**
     * Fragmentシェーダー資源を示す
     */
    VRAM_FragmentShader,

    /**
     * LinkしたShaderProgram資源を示す
     */
    VRAM_ShaderProgram,

    /**
     * フレームバッファオブジェクト
     */
    VRAM_FrameBuffer,

    /**
     * レンダリングバッファオブジェクト
     */
    VRAM_RenderBuffer,

    /**
     * カウンタ用
     */
    VRAM_e_num,

    /**
     * 無効
     */
    VRAM_e_NULL,
};

/**
 * VRAM管理構造体
 */
typedef struct _vram_id {
    /**
     * 管理ID
     */
    GLuint id;

    /**
     * ref counter
     */
    s32 ref;

    _vram_id() {
        id = 0;
        ref = 0;
    }

    ~_vram_id() {
        // refが残ったまま解放することは出来ない
        assert(ref <= 0);
    }
}* vram_id;

class _VideoMemory: public Object {
public:
    typedef void (*vram_alloc_function)(s32 size, u32 *result_array);
    typedef void (*vram_delete_function)(s32 size, u32 *obj_array);

    /**
     * 解放コンテナ
     */
    typedef typename std::vector<GLuint, StlAllocator<GLuint> > dealloc_container;
private:
    /**
     * 排他制御
     */
    jcmutex mutex;

    /**
     * 管理しているメモリタイプ
     */
    VRAM_e type;

    /**
     * 確保済みのプール
     */
    safe_array<GLuint> alloc_pool;

    /**
     * 現在アクセス用のイテレータ
     */
    unsafe_array<GLuint> iterator;

    /**
     * 解放プール
     */
    dealloc_container dealloc_pool;
public:
    _VideoMemory(const VRAM_e type);

    virtual ~_VideoMemory();

    /**
     * 確保するRAMの種類を取得する
     */
    virtual VRAM_e getType() const {
        return type;
    }

    /**
     * 指定したリソースを獲得する
     */
    virtual vram_id alloc();

    /**
     * 参照カウンタをインクリメントする
     */
    virtual vram_id retain(vram_id id);

    /**
     * 参照カウンタをデクリメントする
     */
    virtual void release(vram_id id);

    /**
     * GC対象を取得する
     */
    virtual void getGcTargets(unsafe_array<u32> *result) {
        result->length = dealloc_pool.size();
        result->ptr = &(dealloc_pool[0]);
    }

    /**
     * GCを行う
     */
    virtual void gc();

    /**
     * 開放処理を行う
     */
    virtual void dispose();
};

/**
 * managed
 */
typedef jc_sp<_VideoMemory> VideoMemory;

/**
 * VRAM上に展開されているオブジェクト
 */
class GLObject {
    /**
     * 確保したID
     */
    vram_id id;

    /**
     * RAM
     */
    VideoMemory ram;

    void set(const GLObject &cpy) {
        release();

        if (cpy.id) {
            assert(cpy.ram);
            this->ram = cpy.ram;
            this->id = ram->retain(cpy.id);
        }
    }

public:
    GLObject() {
        id = NULL;
    }

    GLObject(const GLObject &cpy) {
        id = NULL;

        set(cpy);
    }

    ~GLObject() {
        release();
    }

    /**
     * VRAMのメモリ確保を行う
     */
    void alloc(VideoMemory ram) {
        // 既存メモリの解放
        release();

        assert(ram);
        assert(!id);

        this->ram = ram;
        this->id = ram->retain(ram->alloc());
    }

    /**
     * 管理しているメモリの解放を行う
     */
    void release() {
        if (id) {
            assert(ram);

            ram->release(id);
            id = NULL;
            ram.reset();
        }
    }

    /**
     * コピー対策
     */
    GLObject& operator=(const GLObject &cpy) {
        set(cpy);
        return (*this);
    }

    /**
     * IDが同じであれば許可する
     */
    bool operator==(const GLObject &o) {
        return this->id == o.id;
    }

    /**
     * IDが違う
     */
    bool operator!=(const GLObject &o) {
        return this->id != o.id;
    }

    /**
     * 有効な場合はtrueを返す
     */
    operator bool() const {
        return id != 0;
    }

    /**
     * 無効な場合はtrueを返す
     */
    bool operator!() const {
        return id == 0;
    }

    /**
     * VRAM上のオブジェクトIDを取得する
     */
    GLuint get() const {
        if (id) {
            return id->id;
        }
        return 0;
    }

    /**
     * プール対象のRAMを取得する
     */
    VideoMemory getRAM() const {
        return ram;
    }
};

}
}

#endif /* JCVRAM_H_ */
