/*
 * MeshFragment.cpp
 *
 *  Created on: 2012/12/18
 */

#include    "jcfbx/convert/MeshFragment.h"

namespace jc {
namespace fbx {

/**
 *
 */
MeshFragment::MeshFragment() {
    contexts.push_back(MFragmentContext(new FragmentContext()));
    // 最大で利用できるボーン数
    maxBones = 32;
}

/**
 *
 */
MeshFragment::~MeshFragment() {

}

/**
 * 利用できるボーン数をオーバーする場合true
 */
jcboolean MeshFragment::isOverBone(const u8 *bones, const s32 bones_length) {
    std::map<u8, u8> temp;

    // ダブリを防止する
    for (int i = 0; i < bones_length; ++i) {
        temp.insert(std::map<u8, u8>::value_type(bones[i], bones[i]));
    }

    int newBones = 0;
    std::map<u8, u8>::iterator itr = temp.begin(), end = temp.end();

    while (itr != end) {
        if (!isUsingBone(itr->first)) {
            ++newBones;
        }

        ++itr;
    }

    FragmentContext *pContext = getCurrentContext();
    return ((int) pContext->useBoneIndices.size() + newBones) > maxBones;
}

/**
 * 既にボーンを使用済みか
 */
jcboolean MeshFragment::isUsingBone(u8 index) {
    FragmentContext *pContext = getCurrentContext();
    std::map<u8, u8>::iterator itr = pContext->useBoneIndices.begin(), end = pContext->useBoneIndices.end();

    while (itr != end) {

        if (itr->first == index) {
            return jctrue;
        }

        ++itr;
    }

    return jcfalse;
}

/**
 * インデックスバッファを追加する
 */
void MeshFragment::addIndices(const std::vector<FigureVertex> &vertices, const u16 p0, const u16 p1, const u16 p2) {
    int ptr = 0;
    u8 bones[4 * 3] = { };

    // 利用するボーンを列挙する
    {
        const FigureVertex &vertex = vertices[p0];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }
    {
        const FigureVertex &vertex = vertices[p1];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }
    {
        const FigureVertex &vertex = vertices[p2];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }

    if (isOverBone(bones, 12)) {
        // 子を分離させる
        separation();
    }

    // 頂点を登録する
    FragmentContext *pContext = getCurrentContext();
    {
        const FigureVertex &vertex = vertices[p0];
        pContext->indices.push_back(registerVertex(&pContext->vertices, vertex));
    }
    {
        const FigureVertex &vertex = vertices[p1];
        pContext->indices.push_back(registerVertex(&pContext->vertices, vertex));
    }
    {
        const FigureVertex &vertex = vertices[p2];
        pContext->indices.push_back(registerVertex(&pContext->vertices, vertex));
    }

    for (int i = 0; i < 12; ++i) {
        pContext->useBoneIndices.insert(std::map<u8, u8>::value_type(bones[i], bones[i]));
    }
}

/**
 * 現在のデータから分離を行い、新たなフラグメントとして子を作成する。
 */
void MeshFragment::separation() {

    MFragmentContext nextContext(new FragmentContext());
    contexts.push_back(nextContext);
}

}
}

