/*
 * GPU.cpp
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */
#include    "jc/gl/GL.h"
#include    "jc/gl/GPUCapacity.h"
#include    "jc/system/StringUtil.h"
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
static OrderAccessList<String> extensions;

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

static void initialize() {
    initialized = jctrue;

    renderer = (const charactor*) glGetString(GL_RENDERER);
    vendor = (const charactor*) glGetString(GL_VENDOR);
    version = (const charactor*) glGetString(GL_VERSION);

    // エクステンション一覧を取得する
    {
        std::vector<String> temp;
        String tempExtensions = (const charactor*) glGetString(GL_EXTENSIONS);
        split(tempExtensions, " ", &temp);

        for (s32 i = 0; i < temp.size(); ++i) {
            extensions.add(temp[i]);
        }
    }

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, (GLint*) &maxVertexAttrbs);
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*) &maxTextureUnits);
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, (GLint*) &maxTextureSize);
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, (GLint*) &maxUniformVectorsVs);
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, (GLint*) &maxUniformVectorsFs);

    jclog("------------ GPU ------------");
    {
        jclogf("GL_VERSION = %s", version.c_str());
        jclogf("GL_VENDOR = %s", vendor.c_str());
        jclogf("GL_RENDERER = %s", renderer.c_str());

// エクステンション一覧を出力する
        {
            OrderAccessList<String>::Iterator itr = extensions.iterator();
            while (itr->hasNext()) {
                String ext = itr->next();
                jclogf("GL_EXTENSIONS = %s", ext.c_str());
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

}

#define __init(...)     if(!initialized) { initialize(); }

/**
 * レンダラー名を取得する
 */
String GPUCapacity::getRenderer() {
    __init();
    return renderer;
}

/**
 * ベンダー名を取得する
 */
String GPUCapacity::getVendor() {
    __init();
    return vendor;
}

/**
 * 頂点属性の最大数を取得する。
 */
u32 GPUCapacity::getMaxVertexAttributes() {
    __init();
    return maxVertexAttrbs;
}

/**
 * テクスチャユニットの最大数を取得する
 * この枚数を超えるテクスチャ処理は行えない。
 */
u32 GPUCapacity::getMaxTextureUnits() {
    __init();
    return maxTextureUnits;
}

/**
 * テクスチャの一辺のサイズの最大値
 */
u32 GPUCapacity::getMaxTextureSize() {
    __init();
    return maxTextureSize;
}

/**
 * 頂点シェーダのUniformベクトル最大数を取得する
 */
u32 GPUCapacity::getMaxUniformVectorsVs() {
    __init();
    return maxUniformVectorsVs;
}

/**
 * フラグメントシェーダでのUniformベクトル最大数を取得する
 */
u32 GPUCapacity::getMaxUniformVectorsFs() {
    __init();
    return maxUniformVectorsFs;
}
}
}
