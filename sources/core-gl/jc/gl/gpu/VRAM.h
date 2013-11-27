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
    /**
     * VRAMのアクセス用テーブル
     */
    jc_sp<vram_table> vram_tables[VRAM_e_num];



    /**
     * 生成時のミューテックス
     */
    jcmutex mtx;
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

}
}

#endif /* JCVRAM_H_ */
