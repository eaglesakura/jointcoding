/*
 * jcglVRAM.cpp
 *
 *  Created on: 2012/09/25
 */

#include    "jc/gl/GL.h"
#include    "jc/gl/gpu/VRAM.h"
#include    "jc/gl/context/State.h"
#include    <map>

// #define PRINT_VRAM

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

    /**
     * ログに出力する名称
     */
    const charactor *type_name;
};

static VramFunction function_tbl[VRAM_e_num] = {
#if 1
        // VRAM_Texture
        { 16, (vram_alloc_function) glGenTextures, (vram_delete_function) glDeleteTextures, "Texture", },
        // VRAM_Indices
        { 4, (vram_alloc_function) glGenBuffers, (vram_delete_function) glDeleteBuffers, "IndexBuffer", },
        // VRAM_VertexBufferObject
        { 4, (vram_alloc_function) glGenBuffers, (vram_delete_function) glDeleteBuffers, "VertexBuffer", },
        // VRAM_VertexShader
        { 1, (vram_alloc_function) alloc_vshader, (vram_delete_function) delete_vshader, "VertexShader", },
        // VRAM_FragmentShader
        { 1, (vram_alloc_function) alloc_fshader, (vram_delete_function) delete_fshader, "FragmentShader", },
        // VRAM_ShaderProgram
        { 1, (vram_alloc_function) alloc_shader_program, delete_shader_program, "ShaderProgram", },
#endif
// end
        };

_VRAM::_VRAM() {
    for (int i = 0; i < VRAM_e_num; ++i) {
        alloced_num[i] = deleted_num[i] = 0;

    }
}

_VRAM::~_VRAM() {

}

static vram_id get(_VRAM::alloc_list *res, const VRAM_e type) {
    if (!res->empty()) {
        jclogf("cache object(type = %d num(%d))", type, res->size());
    } else {
        // 資源が枯渇しているため、確保を行う
        const s32 alloc_num = function_tbl[type].once_alloc;
        const vram_alloc_function alloc_func = function_tbl[type].alloc_func;

        jclogf("alloc object(type = %d[%d])", type, alloc_num);
        jc_sa<u32> vram_obj(mark_new u32[alloc_num]);

        // 指定数のオブジェクトを確保する
        alloc_func((s32) alloc_num, (u32*) vram_obj.get());

//#ifdef  DEBUG
#if 0
        if (type == VRAM_Texture) {
            static std::map<u32, u32> texture_allocated;

            for (int i = 0; i < alloc_num; ++i) {
                const u32 id = vram_obj[i];
                // まだallocされていないIDだったら
                if (texture_allocated.find(id) == texture_allocated.end()) {
                    texture_allocated.insert(std::pair<u32, u32>(id, id));
                    jclogf("new allocated texture id(%d)", id);
                } else {
                    jclogf("reuse allocated texture id(%d)", id);
                }
            }

        }
#endif

        // GL動作検証
        assert_gl();

        // memory clean
        res->clear();
        // VRAMオブジェクトを追加する
        for (s32 i = 0; i < alloc_num; ++i) {
            vram_id vram = mark_new tagVramId();
            vram->obj = vram_obj[i];
            vram->type = type;
            vram->ref_count = 0;
            res->push_back(vram);
        }
    }

    // 一番手前にあるオブジェクトを返す
    vram_id result = res->front();
//    res->pop_front();
    res->erase(res->begin());
    return result;
}

/**
 * 資源を新規取得する
 * allocした時点でref=1がされているため、新たにref()を行う必要はない。
 */
vram_id _VRAM::alloc(VRAM_e type) {
    MutexLock lock(mutex);
    ++alloced_num[type];

    vram_id id = ref(get(&alloc_pool[type], type));

    // 既に予約中のキャッシュがある場合、それを返さないようにする
    while (existResourceCache[type].exist(id->obj)) {
        jclogf("recreate resource vram(%x) obj(%d)", id, id->obj)

        // 削除プールに戻すと問題が発生するため、delete操作を行う
        SAFE_DELETE(id);

        id = ref(get(&alloc_pool[type], type));
    }

#ifdef PRINT_VRAM
    if (type == VRAM_Texture) {
        jclogf("alloc texture vram obj(%d)", id->obj);
    }
#endif

    // プールに追加する
    existResourceCache[type].insert(id->obj);
    assert(existResourceCache[type].exist(id->obj));

    return id;
}

/**
 * 参照カウントを追加する
 */
vram_id _VRAM::ref(vram_id vid) {
    if (!vid) {
        return NULL;
    }
    MutexLock lock(mutex);

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

    MutexLock lock(mutex);

#ifdef  PRINT_VRAM
    jclogf("release vram(%x = obj:%d  type:%d ref:%d)", vid, vid->obj, vid->type, vid->ref_count);
#endif

    --vid->ref_count;

    // リファレンスが無くなったら削除する
    if (vid->ref_count <= 0) {
        ++deleted_num[vid->type];

#ifdef  PRINT_VRAM
        jclogf("delete  vram(%x = obj:%d  type:%d ref:%d)", vid, vid->obj, vid->type, vid->ref_count)
#endif
        // 解放プールに追加する
        dealloc_pool[vid->type].push_back(vid->obj);

        // 利用中キャッシュからも外す
        existResourceCache[vid->type].remove(vid->obj);
        assert(!existResourceCache[vid->type].exist(vid->obj));

        SAFE_DELETE(vid);
    }
}

/**
 * 管理している資源を全て開放する。
 * GLオブジェクトの生存期間に関わらず強制的に開放されるため、呼び出しを行う際は注意をすること。
 */
void _VRAM::dispose() {
    MutexLock lock(mutex);

    for (s32 i = 0; i < VRAM_e_num; ++i) {
        alloc_list::iterator itr = alloc_pool[i].begin(), end = alloc_pool[i].end();
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
    this->gc(MGLState());
}

/**
 * 不要な資源を解放する。
 */
void _VRAM::gc(MGLState state, const u32 gc_flags) {
    MutexLock lock(mutex);

    int gc_objects = 0;
    for (s32 i = 0; i < VRAM_e_num; ++i) {
        if (has_flag(gc_flags, (0x1 << i))) {
            if (!dealloc_pool[i].empty()) {
                jclogf("delete start(type %s : %d objects)", function_tbl[i].type_name, dealloc_pool[i].size());

                gc_objects += dealloc_pool[i].size();

#if 0
                if (state) {
                    switch (i) {
                        case VRAM_Texture: {
                            state->unbindTextures(dealloc_pool[i].size(), &(dealloc_pool[i][0]));
                        }
                        break;
                    }
                }

                state->finish();
                assert_gl();
#endif

#ifdef PRINT_VRAM
                if (i == VRAM_Texture) {
                    for (int k = 0; k < dealloc_pool[i].size(); ++k) {
                        jclogf("delete texture vram obj(%d)", dealloc_pool[i][k]);
                    }
                }
#endif

                function_tbl[i].delete_func(dealloc_pool[i].size(), &(dealloc_pool[i][0]));
                state->finish();

                assert_gl();
                // プール解放
                dealloc_pool[i].clear();
                jclogf("delete finish(type %d)", i);
            }
        }
    }

    if (gc_objects) {
        jclogf("VRAM-GC(%d objects)", gc_objects);
        if (state) {
            state->syncContext();
        }
    }

}

/**
 * 確保済みのVRAMオブジェクト数を取得する
 */
_VRAM::Infomation _VRAM::getInfo(const VRAM_e vramType) {
    MutexLock lock(mutex);

    Infomation result;

    result.allocated = alloced_num[vramType];
    result.deleted = deleted_num[vramType];
    result.pool = alloc_pool[vramType].size();
    result.gc_target = dealloc_pool[vramType].size();

    return result;
}

}
}

