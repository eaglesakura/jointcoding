/*
 * GPU.cpp
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */
#include    "jc/gl/GL.h"
#include    "jc/gl/gpu/GPUCapacity.h"
#include    "jc/system/StringUtil.h"
#include    "jc/collection/BitFlags.hpp"
#include    <vector>

namespace jc {
namespace gl {

namespace {
static jcboolean initialized = jcfalse;

/**
 * レンダラー名
 */
static String renderer;

/**
 * ベンダー名
 */
static String vendor;

/**
 * バージョン情報
 */
static String version;

/**
 * 保持しているエクステンション
 */
static std::vector<String> extensions;

/**
 * 頂点属性の最大数
 */
static u32 maxVertexAttrbs = 0;

/**
 * テクスチャユニットの最大数
 */
static u32 maxTextureUnits = 0;

/**
 * テクスチャの一辺の最大サイズ
 */
static u32 maxTextureSize = 0;

/**
 * 頂点シェーダでの最大Uniformベクトル数
 */
static u32 maxUniformVectorsVs = 0;

/**
 * フラグメントシェーダでの最大Uniformベクトル数
 */
static u32 maxUniformVectorsFs = 0;

/**
 * 拡張設定
 */
static BitFlags<GPUExtension_Num> extension_flags;

/**
 * GPUファミリー名
 */
static GPUFamily_e gpuFamily = GPUFamily_Unknown;

}

/**
 * 初期化を行う
 */
void GPUCapacity::initialize() {
    if (initialized) {
        return;
    }

    initialized = jctrue;

    {
        // レンダラ名と系列機チェック
        const charactor* pRenderer = (const charactor*) glGetString(GL_RENDERER);
        renderer = pRenderer;

        struct GPUFamilyGroup {
            /**
             * チェックするGPU名
             */
            const charactor* name;

            /**
             * 登録されるファミリー
             */
            GPUFamily_e family;
        } gpu_groups[] = {
        //
        // for PowerVR
                { "PowerVR", GPUFamily_PowerVR },
                // for Mali
                { "Mali", GPUFamily_Mali },
                // for Tegra
                { "Tegra", GPUFamily_Tegra },
                // for Adreno
                { "Adreno", GPUFamily_Adreno }, };

        for (s32 i = 0; i < (sizeof(gpu_groups) / sizeof(GPUFamilyGroup)); ++i) {
            if (strstr(pRenderer, gpu_groups[i].name)) {
                // GPUファミリー
                gpuFamily = gpu_groups[i].family;
                jclogf("GPU = %s(%s)", gpu_groups[i].name, pRenderer);
                break;
            }
        }

        if (gpuFamily == GPUFamily_Unknown) {
            jclogf("GPU = Unknown(%s)", pRenderer);
        }
    }

    vendor = (const charactor*) glGetString(GL_VENDOR);
    version = (const charactor*) glGetString(GL_VERSION);

// エクステンション一覧を取得する
    {
        const charactor *pExtensions = (const charactor*) glGetString(GL_EXTENSIONS);
        split(pExtensions, " ", &extensions);

#define EXTENSION_NAME(def) #def
        const charactor* EXTENSION_NAMES[] = {
        //
        EXTENSION_NAME(GL_OES_compressed_ETC1_RGB8_texture),// ETC1(Android)
        EXTENSION_NAME(GL_IMG_texture_compression_pvrtc), // PVRTC(PowerVR)
                "GL_EXT_texture_compression_s3tc", // s3tc texture(Tegra)
                EXTENSION_NAME(GL_EXT_texture_format_BGRA8888), // BGRA(Android)
                EXTENSION_NAME(GL_OES_EGL_image_external), // SurfaceTexture(Android)
                EXTENSION_NAME(GL_OES_depth_texture), // 深度テクスチャ
                EXTENSION_NAME(GL_EXT_discard_framebuffer), // フレームバッファの無効化
                };

        // 対応している拡張機能を調べる
        for (int i = 0; i < (sizeof(EXTENSION_NAMES) / sizeof(charactor*)); ++i) {
            if (strstr(pExtensions, EXTENSION_NAMES[i])) {
                // check index
                extension_flags.enable(i);
                jclogf("supported extension(%s)", EXTENSION_NAMES[i]);

                assert(GPUCapacity::isSupport((GPUExtension_e)i));
            }
        }
#undef  EXTENSION_NAME

    }

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint*) &maxVertexAttrbs);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*) &maxTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*) &maxTextureSize);
#ifdef GL_MAX_VERTEX_UNIFORM_VECTORS
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, (GLint*) &maxUniformVectorsVs);
#else
    maxUniformVectorsVs = 128;
#endif

#ifdef GL_MAX_FRAGMENT_UNIFORM_VECTORS
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, (GLint*) &maxUniformVectorsFs);
#else
    maxUniformVectorsFs = 128;
#endif

    jclog("------------ GPU ------------");
    {
        jclogf("GL_VERSION = %s", version.c_str());
        jclogf("GL_VENDOR = %s", vendor.c_str());
        jclogf("GL_RENDERER = %s", renderer.c_str());

// エクステンション一覧を出力する
        {
            std::vector<String>::iterator itr = extensions.begin(), end = extensions.end();

            while (itr != end) {
                if ((*itr).length()) {
                    jclogf("GL_EXTENSIONS = %s", (*itr).c_str());
                    ++itr;
                }else {
                    itr = extensions.erase(itr);
                    end = extensions.end();
                }
            }
        }

        jclogf("GL_MAX_VERTEX_ATTRIBS = %d", maxVertexAttrbs);
        jclogf("GL_MAX_TEXTURE_IMAGE_UNITS = %d", maxTextureUnits);
        jclogf("GL_MAX_TEXTURE_SIZE = %d", maxTextureSize);
        jclogf("GL_MAX_VERTEX_UNIFORM_VECTORS = %d", maxUniformVectorsVs);
        jclogf("GL_MAX_FRAGMENT_UNIFORM_VECTORS = %d", maxUniformVectorsFs);
    }
    jclog("-----------------------------");
}

/**
 * レンダラー名を取得する
 */
const String& GPUCapacity::getRenderer() {
    return renderer;
}

/**
 * ベンダー名を取得する
 */
const String& GPUCapacity::getVendor() {
    return vendor;
}

/**
 * 頂点属性の最大数を取得する。
 */
u32 GPUCapacity::getMaxVertexAttributes() {
    return maxVertexAttrbs;
}

/**
 * テクスチャユニットの最大数を取得する
 * この枚数を超えるテクスチャ処理は行えない。
 */
u32 GPUCapacity::getMaxTextureUnits() {
    return maxTextureUnits;
}

/**
 * テクスチャの一辺のサイズの最大値
 */
u32 GPUCapacity::getMaxTextureSize() {
    return maxTextureSize;
}

/**
 * 頂点シェーダのUniformベクトル最大数を取得する
 */
u32 GPUCapacity::getMaxUniformVectorsVs() {
    return maxUniformVectorsVs;
}

/**
 * フラグメントシェーダでのUniformベクトル最大数を取得する
 */
u32 GPUCapacity::getMaxUniformVectorsFs() {
    return maxUniformVectorsFs;
}

/**
 * GPU拡張機能をサポートするかを調べる
 */
jcboolean GPUCapacity::isSupport(const GPUExtension_e extension) {
    return extension_flags.isEnable(extension);
}

/**
 * GPU系列のチェックを行う
 */
GPUFamily_e GPUCapacity::getGPUFamily() {
    return gpuFamily;
}

}
}
