/*
 * SharedVRAM.cpp
 *
 *  Created on: 2013/05/19
 */

#include    "jc/gl/gpu/VRAM.h"
#include    "jc/mem/MemoryUtil.h"
#include    "jc/gl/context/State.h"

namespace jc {
namespace gl {

namespace {

typedef void (*vram_alloc_function)(s32 size, u32 *result_array);
typedef void (*vram_delete_function)(s32 size, u32 *obj_array);

/**
 * 頂点シェーダの確保
 */
static void alloc_vshader(s32 size, u32 *result_array) {
    for (s32 i = 0; i < size; ++i) {
        result_array[i] = glCreateShader(GL_VERTEX_SHADER);
    }
}

/**
 * 頂点シェーダの解放
 */
static void delete_vshader(s32 size, u32 *obj_array) {
    for (s32 i = 0; i < size; ++i) {
        if (obj_array[i]) {
            glDeleteShader(obj_array[i]);
        }
    }
}

/**
 * フラグメントシェーダの確保
 */
static void alloc_fshader(s32 size, u32 *result_array) {
    for (s32 i = 0; i < size; ++i) {
        result_array[i] = glCreateShader(GL_FRAGMENT_SHADER);
    }
}

/**
 * フラグメントシェーダの解放
 */
static void delete_fshader(s32 size, u32 *obj_array) {
    for (s32 i = 0; i < size; ++i) {
        if (obj_array[i]) {
            glDeleteShader(obj_array[i]);
        }
    }
}

/**
 * ShaderProgramを作成する
 */
static void alloc_shader_program(s32 size, u32 *result_array) {
    for (s32 i = 0; i < size; ++i) {
        result_array[i] = glCreateProgram();
    }
}

/**
 * ShaderProgramを削除する
 */
static void delete_shader_program(s32 size, u32 *obj_array) {
    for (s32 i = 0; i < size; ++i) {
        if (obj_array[i]) {
            glDeleteProgram(obj_array[i]);
        }
    }
}

struct VramFunction {
    /**
     * 確保関数
     */
    vram_alloc_function alloc_func;

    /**
     * 解放関数
     */
    vram_delete_function delete_func;
};

static VramFunction function_tbl[VRAM_e_num] = {
// VRAM_Texture
        { (vram_alloc_function) glGenTextures, (vram_delete_function) glDeleteTextures, },
        // VRAM_Indices
        { (vram_alloc_function) glGenBuffers, (vram_delete_function) glDeleteBuffers, },
        // VRAM_VertexBufferObject
        { (vram_alloc_function) glGenBuffers, (vram_delete_function) glDeleteBuffers, },
        // VRAM_VertexShader
        { (vram_alloc_function) alloc_vshader, (vram_delete_function) delete_vshader, },
        // VRAM_FragmentShader
        { (vram_alloc_function) alloc_fshader, (vram_delete_function) delete_fshader, },
        // VRAM_ShaderProgram
        { (vram_alloc_function) alloc_shader_program, (vram_delete_function) delete_shader_program, },
        // VRAM_FrameBuffer
        { (vram_alloc_function) glGenFramebuffers, (vram_delete_function) glDeleteFramebuffers },
        // VRAM_RenderBuffer
        { (vram_alloc_function) glGenRenderbuffers, (vram_delete_function) glDeleteRenderbuffers },
// end
        };
}

SharedVRAM::SharedVRAM() {
    for (int i = 0; i < VRAM_e_num; ++i) {
        gc_tables[i].reset(new vram_table());
    }
}

SharedVRAM::~SharedVRAM() {

}
/**
 * マスターVRAMのスレイブとして動作させる
 * 共有可能なオブジェクトはメモリ空間を共有される。
 * ステート情報は共有されないことに注意すること
 */
void SharedVRAM::sharedFrom(jc_sp<SharedVRAM> master) {
    const VRAM_e shared_obj[] = {
        // テクスチャ
        VRAM_Texture,
        // インデックスバッファ
        VRAM_Indices,
        // 頂点バッファ
        VRAM_VertexBufferObject,
    };

// 隷属を宣言する
    for( int i = 0; i < (sizeof(shared_obj) / sizeof(VRAM_e)); ++i) {
        const VRAM_e type = shared_obj[i];
        // GCテーブルを共有する
        gc_tables[type] = master->gc_tables[type];
    }
}

vram_handle SharedVRAM::alloc(const VRAM_e type) {
    MutexLock lock(mtx);

    tagvram_id *p = new tagvram_id();
    assert(p);

    // 確保を行う
    {
        assert(function_tbl[type].alloc_func);
        function_tbl[type].alloc_func(1, &p->handle);
        assert(p->handle);
    }

    vram_handle result(p);
    // gcテーブルに戻りを追加する
    gc_tables[type]->push_back(result);

    // 想定通りの管理数であることを保証する
    assert(result.refs() == 2);
    return result;
}

/**
 * 不要なオブジェクトの掃除を行う
 */
void SharedVRAM::gc(const u32 gc_flags) {
    MutexLock lock(mtx);

    for (int i = 0; i < VRAM_e_num; ++i) {
        if (gc_flags & (0x1 << i)) {
            // 廃棄が確定したIDを列挙する
            std::vector<GLuint> destroy_table;

            std::vector<vram_handle>::iterator itr = gc_tables[i]->begin(), end = gc_tables[i]->end();
            while (itr != end) {
                // GCターゲットでしか管理していない場合、廃棄対象である
                if (itr->refs() <= 1) {
                    destroy_table.push_back(itr->get());
                    itr = gc_tables[i]->erase(itr);
                    end = gc_tables[i]->end();
                } else {
                    ++itr;
                }
            }

            if (!destroy_table.empty()) {
                function_tbl[i].delete_func(destroy_table.size(), (u32*) (&(destroy_table[0])));
            }

            jclogf("vram gc type(%d) obj(%d)", i, destroy_table.size());
        }
    }
}

/**
 * 資源の完全な解放を行う
 */
void SharedVRAM::dispose() {
    MutexLock lock(mtx);

    for (int i = 0; i < VRAM_e_num; ++i) {
        jclogf("vram dispose type(%d) obj(%d)", i, gc_tables[i]->size());
    }

}

}

}
