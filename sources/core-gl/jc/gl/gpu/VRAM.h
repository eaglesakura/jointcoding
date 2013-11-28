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
 * gc()を行う際の挙動を設定する。
 */
enum VRAM_GC_e {
    /**
     * テクスチャ資源を解放する
     */
    VRAM_GC_Texture = 0x1 << VRAM_Texture,

    /**
     * インデックスバッファ資源を解放する
     */
    VRAM_GC_Indices = 0x1 << VRAM_Indices,

    /**
     * VBOを解放する
     */
    VRAM_GC_VertexBufferObjct = 0x1 << VRAM_VertexBufferObject,

    /**
     * シェーダを解放する
     */
    VRAM_GC_VertexShader = 0x1 << VRAM_VertexShader,

    /**
     * シェーダを解放する
     */
    VRAM_GC_FragmentShader = 0x1 << VRAM_FragmentShader,

    /**
     * リンクされたシェーダーを解放する
     */
    VRAM_GC_ShaderProgram = 0x1 << VRAM_ShaderProgram,

    /**
     * リンクされたシェーダーを解放する
     */
    VRAM_GC_FrameBuffer = 0x1 << VRAM_FrameBuffer,
    /**
     * リンクされたシェーダーを解放する
     */
    VRAM_GC_RenderBuffer = 0x1 << VRAM_RenderBuffer,

    /**
     * デフォルトの挙動（全資源一括解放）を行う。
     */
    VRAM_GC_default = 0xFFFFFFFF,
};

typedef handle_table<GLuint> vram_table;

typedef Handle<GLuint> vram_handle;

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

/**
 * Shared Contextを前提としたVRAMクラス
 * 共有可能なものは共有し、それ以外は独自でハンドル管理する
 *
 * 複数スレッドからアクセスされる危険性があるため、生成時・廃棄時にはmutexを使った排他処理を行う
 */
class SharedVRAM: public Object {
public:
    typedef typename std::vector<vram_id, StlAllocator<vram_id> > alloc_list;
    typedef typename std::vector<u32, StlAllocator<u32> > dealloc_list;
private:
    /**
     * VRAMのアクセス用テーブル
     */
    jc_sp<vram_table> vram_tables[VRAM_e_num];

    /**
     * 生成時のミューテックス
     */
    jcmutex mutex;

    /**
     * 生成済みのvram_id一覧
     */
    alloc_list alloc_pool[VRAM_e_num];

    /**
     * 廃棄用のVRAM領域
     */
    dealloc_list dealloc_pool[VRAM_e_num];

public:
    SharedVRAM();

    virtual ~SharedVRAM();

    /**
     * マスターVRAMのスレイブとして動作させる
     * 共有可能なオブジェクトはメモリ空間を共有される。
     * ステート情報は共有されないことに注意すること
     */
    void sharedFrom( jc_sp<SharedVRAM> master);

    /**
     * 領域確保を行う
     */
    vram_handle alloc(const VRAM_e type);

    /**
     * 参照カウンタを1つ上げる
     */
    vram_id retain(vram_id id);

    /**
     * 参照カウンタを1つ下げる
     */
    void release(vram_id id);

    /**
     * 不要なオブジェクトの掃除を行う
     */
    void gc(const u32 gc_flags = VRAM_GC_default);

    /**
     * 管理しているオブジェクトの完全な廃棄を行う
     */
    void dispose();
};

/**
 * managed
 */
typedef jc_sp<SharedVRAM> VRAM;

class VideoMemory: public Object {
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
    VideoMemory(const VRAM_e type);

    virtual ~VideoMemory();

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
typedef jc_sp<VideoMemory> MVideoMemory;

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
    MVideoMemory ram;

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
    void alloc(MVideoMemory ram) {
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
    MVideoMemory getRAM() const {
        return ram;
    }
};

}
}

#endif /* JCVRAM_H_ */
