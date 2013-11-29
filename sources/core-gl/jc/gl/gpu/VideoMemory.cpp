/*
 * VideoMemory.cpp
 *
 *  Created on: 2013/11/28
 */

#include    "jc/gl/gpu/VRAM.h"

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
// VRAM_Texture
        { 32, (vram_alloc_function) glGenTextures, (vram_delete_function) glDeleteTextures, "Texture", },
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
        // VRAM_FrameBuffer
        { 1, (vram_alloc_function) glGenFramebuffers, (vram_delete_function) glDeleteFramebuffers, "FrameBuffer" },
        // VRAM_RenderBuffer
        { 1, (vram_alloc_function) glGenRenderbuffers, (vram_delete_function) glDeleteRenderbuffers, "RenderBuffer", } };

// end

_VideoMemory::_VideoMemory(const VRAM_e type) {
    this->type = type;

    // 一括取得量を設定する
    this->alloc_pool.reserve(function_tbl[type].once_alloc);
}

_VideoMemory::~_VideoMemory() {

}

/**
 * 指定したリソースを獲得する
 */
vram_id _VideoMemory::alloc() {
    MutexLock lock(mutex);

    vram_id result = mark_new _vram_id();
    result->ref = 0;

    if (!iterator) {
        // 取得済み残量が残っていない
        vram_alloc_function alloc_func = function_tbl[type].alloc_func;
        // 確保を行う
        alloc_func(alloc_pool.length, alloc_pool.ptr);

        // イテレータを作成する
        iterator = alloc_pool.iterator();
    }

    // 取得済み残量がまだ残ってる
    result->id = (*iterator);
    // イテレータを進める
    ++iterator;
    return result;
}

/**
 * 参照カウンタをインクリメントする
 */
vram_id _VideoMemory::retain(vram_id id) {
    assert(id);
    MutexLock lock(mutex);
    ++id->ref;

    return id;
}

/**
 * 参照カウンタをデクリメントする
 */
void _VideoMemory::release(vram_id id) {
    assert(id);
    MutexLock lock(mutex);

    --id->ref;

    // 誰からも参照されなくなったら解放プールへ追加
    if (id->ref <= 0) {
        jclogf("dealloc type[%s] id(%d)", function_tbl[type].type_name, id->id);

        dealloc_pool.push_back(id->id);
        // メモリ解放
        SAFE_DELETE(id);
    }
}

/**
 * GCを行う
 */
void _VideoMemory::gc() {
    MutexLock lock(mutex);

    // 解放関数を呼び出す
    if (!dealloc_pool.empty()) {
        const u32 length = dealloc_pool.size();

        // 取得済み残量が残っていない
        vram_delete_function dealloc_func = function_tbl[type].delete_func;
        dealloc_func(length, &(dealloc_pool[0]));
        glFinish();

        dealloc_pool.clear();

        jclogf("gc[%s][%d objects]", function_tbl[type].type_name, length);
    }
}

/**
 * 開放処理を行う
 */
void _VideoMemory::dispose() {
    MutexLock lock(mutex);

    // 残っているGLオブエジェクトを解放リストに乗せる
    while (iterator) {
        dealloc_pool.push_back(*iterator);
        ++iterator;
    }

    gc();
}

}
}
