/*
 * SharedVRAM.cpp
 *
 *  Created on: 2013/05/19
 */

#include    "jc/gl/gpu/VRAM.h"

namespace jc {
namespace gl {
/**
 * テクスチャ用コールバック関数
 */
void vramtable_texture_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated:
            // 生成を行う
            glGenTextures(objects, pValues);
            break;
        case HandleCallback_Released:
            // 廃棄を行う
            // 廃棄は別途gc命令で行う
            break;
    }
}
/**
 * インデックスバッファ用コールバック関数
 */
void vramtable_indices_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated:
            // 生成を行う
            glGenBuffers(objects, pValues);
            break;
        case HandleCallback_Released:
            // 廃棄を行う
            // 廃棄は別途gc命令で行う
            break;
    }
}
/**
 * VBO用コールバック関数
 */
void vramtable_vbo_callback(const HandleCallback_e callback, void* pHandleTable_this, GLuint* pValues, handle_meta *pMetaHeader, const u32 objects) {
    switch (callback) {
        case HandleCallback_Allocated:
            // 生成を行う
            glGenBuffers(objects, pValues);
            break;
        case HandleCallback_Released:
            // 廃棄を行う
            // 廃棄は別途gc命令で行う
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
        }
            break;
        case HandleCallback_Released:
            // 廃棄を行う
            // 廃棄は別途gc命令で行う
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
        }
            break;
        case HandleCallback_Released:
            // 廃棄を行う
            // 廃棄は別途gc命令で行う
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
        case HandleCallback_Released:
            // 廃棄を行う
            // 廃棄は別途gc命令で行う
            break;
    }
}

SharedVRAM::SharedVRAM() {
    {

        shared.textures.reset(new vram_table());
        shared.indexbuffers.reset(new vram_table());
        shared.vertexbuffers.reset(new vram_table());

        non_shared.vert_shader.reset(new vram_table());
        non_shared.frag_shader.reset(new vram_table());
        non_shared.programs.reset(new vram_table());

        // 確保用のオブジェクトのテーブルを作成する
        vram_tables[VRAM_Texture] = shared.textures;
        vram_tables[VRAM_Indices] = shared.indexbuffers;
        vram_tables[VRAM_VertexBufferObject] = shared.vertexbuffers;
        vram_tables[VRAM_VertexShader] = non_shared.vert_shader;
        vram_tables[VRAM_FragmentShader] = non_shared.frag_shader;
        vram_tables[VRAM_ShaderProgram] = non_shared.programs;
    }

    // コールバック設定を行う
    {
        // 共有オブジェクト
        shared.textures->setCallback((handletable_callback) vramtable_texture_callback);
        shared.indexbuffers->setCallback((handletable_callback) vramtable_indices_callback);
        shared.vertexbuffers->setCallback((handletable_callback) vramtable_vbo_callback);

        // 非共有オブジェクト
        non_shared.vert_shader->setCallback((handletable_callback) vramtable_vertshader_callback);
        non_shared.frag_shader->setCallback((handletable_callback) vramtable_fragshader_callback);
        non_shared.programs->setCallback((handletable_callback) vramtable_program_callback);
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
    // FIXME GC処理を追加する
}

/**
 * 資源の完全な解放を行う
 */
void SharedVRAM::dispose() {
    // FIXME 廃棄処理を追加する
}

}

}
