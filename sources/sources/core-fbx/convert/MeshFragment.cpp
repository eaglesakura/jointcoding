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
    // 最大で利用できるボーン数
    maxBones = 12;
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

    return ((int) useBoneIndices.size() + newBones) > maxBones;
}

/**
 * 既にボーンを使用済みか
 */
jcboolean MeshFragment::isUsingBone(u8 index) {
    std::map<u8, u8>::iterator itr = useBoneIndices.begin(), end = useBoneIndices.end();

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
void MeshFragment::addIndices(const std::vector<SimpleVertex> &vertices, const u16 p0, const u16 p1, const u16 p2) {

    int ptr = 0;
    u8 bones[4 * 3] = { };

    // 利用するボーンを列挙する
    {
        const SimpleVertex &vertex = vertices[p0];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }
    {
        const SimpleVertex &vertex = vertices[p1];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }
    {
        const SimpleVertex &vertex = vertices[p2];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }

    if (isOverBone(bones, 12)) {
        jclog("bones over!!");
    }

    indices.push_back(p0);
    indices.push_back(p1);
    indices.push_back(p2);

    for (int i = 0; i < 12; ++i) {
        useBoneIndices.insert(std::map<u8, u8>::value_type(bones[i], bones[i]));
    }
}

}
}

