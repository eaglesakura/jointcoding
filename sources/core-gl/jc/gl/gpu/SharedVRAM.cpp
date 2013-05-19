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
        case HandleCallback_Released: {
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
        case HandleCallback_Released: {
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
            }
            assert_gl();
        }
            break;
        case HandleCallback_Released: {
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
        case HandleCallback_Released: {
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
        case HandleCallback_Released: {
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
            (handletable_callback) vramtable_program_callback, };

    for (int i = 0; i < VRAM_e_num; ++i) {
        vram_tables[i].reset(new vram_table());
        vram_tables[i]->setCallback(callback_functions[i]);
    }
}

SharedVRAM::~SharedVRAM() {

}

/**
 * 領域確保を行う
 */
vram_handle SharedVRAM::alloc(const VRAM_e type) {
    vram_handle result;
    result.reset(vram_tables[type]);
    result.alloc();
    return result;
}

/**
 * 不要なオブジェクトの掃除を行う
 */
void SharedVRAM::gc(const u32 gc_flags) {
    for (s32 i = 0; i < VRAM_e_num; ++i) {
        if (gc_flags & (0x1 << i)) {
            s32 unused = 0;
            vram_tables[i]->getTable()->executeExistOrUnused(&unused);
            jclogf("vram gc type(%d) obj(%d)", i, unused);
        }
    }
}

/**
 * 資源の完全な解放を行う
 */
void SharedVRAM::dispose() {
    for (s32 i = 0; i < VRAM_e_num; ++i) {
        jclogf("vram dispose(%d)", vram_tables[i]->getTable()->getExistValueNum());
        vram_tables[i].reset();
    }

}

}

}
