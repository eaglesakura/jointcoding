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

/**
 * テクスチャ用コールバック関数
 */
void vramtable_texture_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated: {
            // 生成を行う
            glGenTextures(objects, pValues);
            assert_gl();
        }
            break;
        case HandleCallback_Unused: {
            // 廃棄を行う
            glDeleteTextures(objects, pValues);
            assert_gl();
            zeromemory(pValues, sizeof(GLuint) * objects);

            jclogf("vram delete %d objects", objects);
        }
            break;
    }
}
/**
 * インデックスバッファ用コールバック関数
 */
void vramtable_indices_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated: {
            // 生成を行う
            glGenBuffers(objects, pValues);
            assert_gl();
        }
            break;
        case HandleCallback_Unused: {
            // 廃棄を行う
            glDeleteBuffers(objects, pValues);
            assert_gl();
            zeromemory(pValues, sizeof(GLuint) * objects);

            jclogf("vram delete  %d objects", objects);
        }
            break;
    }
}
/**
 * VBO用コールバック関数
 */
void vramtable_vbo_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated: {
            // 生成を行う
            glGenBuffers(objects, pValues);
            assert_gl();
        }
            break;
        case HandleCallback_Unused: {
            // 廃棄を行う
            glDeleteBuffers(objects, pValues);
            assert_gl();
            zeromemory(pValues, sizeof(GLuint) * objects);

            jclogf("vram delete  %d objects", objects);
        }
            break;
    }
}
/**
 * 頂点シェーダ用コールバック関数
 */
void vramtable_vertshader_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated: {
            // 生成を行う
            for (int i = 0; i < objects; ++i) {
                pValues[i] = glCreateShader(GL_VERTEX_SHADER);
                assert(pValues[i]);
            }
            assert_gl();
        }
            break;
        case HandleCallback_Unused: {
            // 廃棄を行う
            for (int i = 0; i < objects; ++i) {
                glDeleteShader(pValues[i]);
            }
            assert_gl();
            zeromemory(pValues, sizeof(GLuint) * objects);

            jclogf("vram delete  %d objects", objects);
        }
            break;
    }
}

/**
 * フラグメントシェーダ用コールバック関数
 */
void vramtable_fragshader_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated: {
            // 生成を行う
            for (int i = 0; i < objects; ++i) {
                pValues[i] = glCreateShader(GL_FRAGMENT_SHADER);
            }
            assert_gl();
        }
            break;
        case HandleCallback_Unused: {
            // 廃棄を行う
            for (int i = 0; i < objects; ++i) {
                glDeleteShader(pValues[i]);
            }
            assert_gl();
            zeromemory(pValues, sizeof(GLuint) * objects);

            jclogf("vram delete  %d objects", objects);
        }
            break;
    }
}

/**
 * シェーダープログラム用コールバック関数
 */
void vramtable_program_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated: {
            // 生成を行う
            for (int i = 0; i < objects; ++i) {
                pValues[i] = glCreateProgram();
            }
        }
            break;
        case HandleCallback_Unused: {
            // 廃棄を行う
            for (int i = 0; i < objects; ++i) {
                glDeleteProgram(pValues[i]);
            }
            assert_gl();
            zeromemory(pValues, sizeof(GLuint) * objects);

            jclogf("vram delete  %d objects", objects);
        }
            break;
    }
}

/**
 * フレームバッファ用コールバック関数
 */
void vramtable_framebuffer_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated: {
            // 生成を行う
            glGenFramebuffers(objects, pValues);
        }
            break;
        case HandleCallback_Unused: {
            // 廃棄を行う
            glDeleteFramebuffers(objects, pValues);
            assert_gl();
            zeromemory(pValues, sizeof(GLuint) * objects);

            jclogf("vram delete  %d objects", objects);
        }
            break;
    }
}

/**
 * レンダリングバッファ用コールバック関数
 */
void vramtable_renderbuffer_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated: {
            // 生成を行う
            glGenRenderbuffers(objects, pValues);
        }
            break;
        case HandleCallback_Unused: {
            // 廃棄を行う
            glDeleteRenderbuffers(objects, pValues);
            assert_gl();
            zeromemory(pValues, sizeof(GLuint) * objects);

            jclogf("vram delete  %d objects", objects);
        }
            break;
    }
}

SharedVRAM::SharedVRAM() {

    const handletable_callback callback_functions[] = {
    // textures
            (handletable_callback) vramtable_texture_callback,
            // indices
            (handletable_callback) vramtable_indices_callback,
            // vbo
            (handletable_callback) vramtable_vbo_callback,
            // vert shader
            (handletable_callback) vramtable_vertshader_callback,
            // frag shader
            (handletable_callback) vramtable_fragshader_callback,
            // program
            (handletable_callback) vramtable_program_callback,
            // framebuffer
            (handletable_callback) vramtable_framebuffer_callback,
            // renderbufffer
            (handletable_callback) vramtable_renderbuffer_callback, };

    for (int i = 0; i < VRAM_e_num; ++i) {
        vram_tables[i].reset(new vram_table());
        vram_tables[i]->setCallback(callback_functions[i]);
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

    };

    // 隷属を宣言する
    for( s32 i = 0; i < (sizeof(shared_obj) / sizeof(VRAM_e)); ++i) {
        const VRAM_e type = shared_obj[i];
        vram_tables[type] = master->vram_tables[type];
    }
}

/**
 * 領域確保を行う
 */
vram_handle SharedVRAM::alloc(const VRAM_e type) {
    MutexLock lock(mutex);

    vram_handle result;
    result.reset(vram_tables[type]);
    result.alloc();

    assert(result.get());
    assert_gl();
    return result;
}

/**
 * 参照カウンタを1つ上げる
 */
vram_id SharedVRAM::retain(vram_id id) {
    assert(id);

    MutexLock lock(mutex);
    ++id->ref;
    return id;
}

/**
 * 参照カウンタを1つ下げる
 */
void SharedVRAM::release(vram_id id) {
    assert(id);

    MutexLock lock(mutex);
    --id->ref;

    // 参照先が無くなったらメモリ解放
    if (id->ref <= 0) {
        dealloc_pool[id->type].push_back(id->id);
        SAFE_DELETE(id);
    }
}

/**
 * 不要なオブジェクトの掃除を行う
 */
void SharedVRAM::gc(const u32 gc_flags) {
    MutexLock lock(mutex);

    for (s32 i = 0; i < VRAM_e_num; ++i) {
        if (gc_flags & (0x1 << i)) {
            s32 unused = 0;
            vram_tables[i]->executeExistOrUnused(&unused);
            jclogf("vram gc type(%d) obj(%d)", i, unused);
        }
    }
}

/**
 * 資源の完全な解放を行う
 */
void SharedVRAM::dispose() {
    MutexLock lock(mutex);

    for (s32 i = 0; i < VRAM_e_num; ++i) {
        jclogf("vram dispose type(%d) obj(%d)", i, vram_tables[i]->getExistValueNum());
        vram_tables[i]->unuse();
        vram_tables[i].reset();
    }

}

}

}
