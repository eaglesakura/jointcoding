/*
 * jcGLVRAM.h
 *
 *  Created on: 2012/08/22
 */

#ifndef JCGLVRAM_H_
#define JCGLVRAM_H_

#include    "jc/system/Macro.h"
#include    "jc/mem/SmartPtr.h"
#include    <vector>
#include    <list>

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
     * カウンタ用
     */
    VRAM_e_num,

    /**
     * 無効
     */
    VRAM_e_NULL,
};

/**
 * VRAM資源を管理する。
 * vram_idの生存期間はVRAMの生存期間と同一であるため、扱いには十分に注意すること。
 *
 * vram_id自体はEGLContextと結びつき、EGLContext廃棄時に同時に廃棄される。
 * EGLContextと一蓮托生な理由は、Contextが廃棄された時点でVRAMの内容が同期的に廃棄されるため。
 */
struct tagVramId {
    /**
     * VRAM上のリソースID
     */
    u32 obj;

    /**
     * VRAMのリソースタイプ
     */
    VRAM_e type;

    /**
     * 参照カウント。
     * リファレンスは手動で行うため、扱いに注意する。
     */
    s32 ref_count;
};

typedef tagVramId* vram_id;

/**
 * VRAMがNULLか否かを判断する
 */
#define __IS_NULL(vid)        (!vid || (vid->ref_count <= 0))

/**
 * vram_idからGL用オブジェクトに変換する
 */
inline const u32 vram2obj(const vram_id vid) {
    return __IS_NULL(vid) ? 0 : vid->obj;
}

/**
 * vram_idからGL用オブジェクトに変換する
 */
inline const VRAM_e vram2type(const vram_id vid) {
    return __IS_NULL(vid) ? VRAM_e_NULL : vid->type;
}

/**
 * VRAM用のメモリが生存していることを確認する
 */
inline const jcboolean vram_is_exist(const vram_id vid) {
    return __IS_NULL(vid) ? jcfalse : jctrue;
}

#undef __IS_NULL

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
     * デフォルトの挙動（全資源一括解放）を行う。
     */
    VRAM_GC_default = 0xFFFFFFFF,
};

/**
 * VRAM上のオブジェクトalloc/ref/releaseを管理する。
 * SmartPtrに比べて、VRAM資源管理用に最適化を行う。
 * VRAMはEGLContextと一蓮托生で、EGLが生存中は必ず生存を行わせる。
 * vram_idを握っているオブジェクトは必ずVRAMの生存期間に注意すること。
 *
 * TODO VRAMで保持するデータは複数コンテキストで共有されることを想定して書きなおす
 * 各変数を別クラス化して、shareできるようにすれば問題ない
 */
class _VRAM {

    /**
     * 今までに確保した合計量を保存しておく
     */
    s32 alloced_num[VRAM_e_num];

    /**
     * 今までに削除した合計量を保存しておく
     */
    s32 deleted_num[VRAM_e_num];

    /**
     * 生成済みのvram_id一覧
     */
    std::list<vram_id> alloc_pool[VRAM_e_num];

    /**
     * 廃棄用のVRAM領域
     */
    std::vector<u32> dealloc_pool[VRAM_e_num];
public:
    _VRAM();
    virtual ~_VRAM();

    /**
     * 資源を新規取得する
     * allocした時点でref=1がされているため、新たにref()を行う必要はない。
     */
    virtual vram_id alloc(VRAM_e type);

    /**
     * 参照カウントを追加する
     */
    virtual vram_id ref(vram_id vid);

    /**
     * 資源を開放する。
     * 資源は再利用前提で、gc()が行われるまで保持される。
     * VRAMは一括取得・一括解放のほうが効率的に利用できるため。
     */
    virtual void release(vram_id vid);

    /**
     * 管理している資源を全て開放する。
     * GLオブジェクトの生存期間に関わらず強制的に開放されるため、呼び出しを行う際は注意をすること。
     */
    virtual void dispose();

    /**
     * 不要な資源をまとめて解放する。
     */
    virtual void gc(const u32 gc_flags = VRAM_GC_default);

    /**
     *
     */
    struct Infomation {
        /**
         * 確保済みのオブジェクト数
         */
        u32 allocated;

        /**
         * 不要となったオブジェクト数
         */
        u32 deleted;

        /**
         * プール数
         */
        u32 pool;

        /**
         * 削除待ち
         */
        u32 gc_target;
    };

    /**
     * 確保済みのVRAMオブジェクト数を取得する
     */
    virtual _VRAM::Infomation getInfo(const VRAM_e vramType);
};

/**
 * unmanaged
 */
typedef _VRAM* VRAM_PTR;

/**
 * managed
 */
typedef jc_sp<_VRAM> VRAM;

/**
 * VRAMリソースを管理する。
 */
class SharedResource {
    VRAM vram;
    vram_id vid;
public:
    SharedResource() {
        this->vid = NULL;
    }

    /**
     * コピーコンストラクタ
     */
    SharedResource(const SharedResource &cpy) {
        this->vram = cpy.vram;
        this->vid = vram->ref(cpy.vid);
    }

    ~SharedResource() {
        this->release();
    }

    /**
     * 管理している資源を解放する
     */
    void release() {
        if (vid) {
            vram->release(vid);
            vid = NULL;
        }
        vram.reset();
    }

    /**
     * コピーを行う
     */
    SharedResource& operator=(const SharedResource &cpy) {
        release();
        this->vram = cpy.vram;
        this->vid = vram->ref(cpy.vid);
        return (*this);
    }

    /**
     * VRAMを再度確保する
     */
    vram_id alloc(VRAM vram, const VRAM_e type) {
        // 握っているリソースを解放する
        release();
        this->vram = vram;
        this->vid = vram->alloc(type);
        return vid;
    }

    /**
     * VRAMリソースを取得する
     */
    u32 get() const {
        return vram2obj(vid);
    }

    /**
     * VRAMのタイプを取得する。
     */
    VRAM_e type() const {
        return vram2type(vid);
    }

    vram_id getId() const {
        return vid;
    }

    /**
     * VRAMが生きているならtrueを返す。
     */
    jcboolean exist() const {
        return vram_is_exist(vid);
    }
};

}
}

#endif /* JCVRAM_H_ */
