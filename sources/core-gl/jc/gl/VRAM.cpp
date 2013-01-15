/*
 * jcglVRAM.cpp
 *
 *  Created on: 2012/09/25
 */

#include    "jc/gl/VRAM.h"
#include    "jc/gl/GL.h"

//     #define PRINT_VRAM

namespace jc {
namespace gl {

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
     * 一度に確保する量
     */
    s32 once_alloc;

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
#if 1
        // VRAM_Texture
        { 16, (vram_alloc_function) glGenTextures, (vram_delete_function) glDeleteTextures, },
        // VRAM_Indices
        { 4, (vram_alloc_function) glGenBuffers, (vram_delete_function) glDeleteBuffers, },
        // VRAM_VertexBufferObject
        { 4, (vram_alloc_function) glGenBuffers, (vram_delete_function) glDeleteBuffers, },
        // VRAM_VertexShader
        { 1, (vram_alloc_function) alloc_vshader, (vram_delete_function) delete_vshader, },
        // VRAM_FragmentShader
        { 1, (vram_alloc_function) alloc_fshader, (vram_delete_function) delete_fshader, },
        // VRAM_ShaderProgram
        { 1, (vram_alloc_function) alloc_shader_program, delete_shader_program, },
#endif
// end
        };

_VRAM::_VRAM() {
}

_VRAM::~_VRAM() {

}

static vram_id get(std::list<vram_id> res, VRAM_e type) {

    if (!res.empty()) {
        // 資源が枯渇してなければ、最初のオブジェクトを取り出して返す。
        vram_id result = res.front();
        res.pop_front();
        return result;
    } else {
        // 資源が枯渇しているため、確保を行う
        const s32 alloc_num = function_tbl[type].once_alloc;
        const vram_alloc_function alloc_func = function_tbl[type].alloc_func;

        jclogf("alloc object(type = %d[%d])", type, alloc_num);
        jc_sa<u32> vram_obj(new u32[alloc_num]);

        // 指定数のオブジェクトを確保する
        alloc_func((s32) alloc_num, (u32*) vram_obj.get());

        // VRAMオブジェクトを追加する
        for (s32 i = 0; i < alloc_num; ++i) {
            vram_id vram = new tagVramId();
            vram->obj = vram_obj[i];
            vram->type = type;
            vram->ref_count = 0;
            res.push_back(vram);
        }

        // 一番手前にあるオブジェクトを返す
        vram_id result = res.front();
        res.pop_front();
        return result;
    }
}

/**
 * 資源を新規取得する
 * allocした時点でref=1がされているため、新たにref()を行う必要はない。
 */
vram_id _VRAM::alloc(VRAM_e type) {
    return ref(get(this->alloc_pool[type], type));
}

/**
 * 参照カウントを追加する
 */
vram_id _VRAM::ref(vram_id vid) {
    if (!vid) {
        return NULL;
    }
    ++vid->ref_count;
#ifdef  PRINT_VRAM
    jclogf("ref     vram(%x = obj:%d  type:%d ref:%d)", vid, vid->obj, vid->type, vid->ref_count)
#endif
    return vid;
}

/**
 * 資源を開放する。
 * 資源は再利用前提で、gc()が行われるまで保持される。
 * VRAMは一括取得・一括解放のほうが効率的に利用できるため。
 */
void _VRAM::release(vram_id vid) {
    if (!vid) {
        return;
    }

#ifdef  PRINT_VRAM
    jclogf("release vram(%x = obj:%d  type:%d ref:%d)", vid, vid->obj, vid->type, vid->ref_count);
#endif

    --vid->ref_count;

    // リファレンスが無くなったら削除する
    if (vid->ref_count <= 0) {
#ifdef  PRINT_VRAM
        jclogf("delete  vram(%x = obj:%d  type:%d ref:%d)", vid, vid->obj, vid->type, vid->ref_count)
#endif
        // 解放プールに追加する
        dealloc_pool[vid->type].push_back(vid->obj);
        SAFE_DELETE(vid);
    }
}

/**
 * 管理している資源を全て開放する。
 * GLオブジェクトの生存期間に関わらず強制的に開放されるため、呼び出しを行う際は注意をすること。
 */
void _VRAM::dispose() {
    for (s32 i = 0; i < VRAM_e_num; ++i) {
        std::list<vram_id>::iterator itr = alloc_pool[i].begin(), end = alloc_pool[i].end();
        while (itr != end) {
            // オブジェクトを解放する
            {
                vram_id vid = (*itr);
                this->release(vid);
            }
            ++itr;
        }
    }

    // まとめて解放を行う
    this->gc();
}

/**
 * 不要な資源を解放する。
 */
void _VRAM::gc(const u32 gc_flags) {
    int gc_objects = 0;
    for (s32 i = 0; i < VRAM_e_num; ++i) {
        if (has_flag(gc_flags, (0x1 << i))) {
            if (!dealloc_pool[i].empty()) {
                jclogf("delete start(type %d : %d objects)", i, dealloc_pool[i].size());

                gc_objects += dealloc_pool[i].size();

                // 解放ビットフラグを含んでいたら、解放を行う
                function_tbl[i].delete_func((s32) dealloc_pool[i].size(), (u32*) &(dealloc_pool[i][0]));

                // プール解放
                dealloc_pool[i].clear();
                jclogf("delete finish(type %d)", i);
            }
        }
    }

    jclogf("VRAM-GC(%d objects)", gc_objects);
}

}
}

