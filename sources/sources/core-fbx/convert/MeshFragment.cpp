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

}

/**
 *
 */
MeshFragment::~MeshFragment() {

}

/**
 * インデックスバッファを追加する
 */
void MeshFragment::addIndices(const std::vector<SimpleVertex> &vertices, const u16 p0, const u16 p1, const u16 p2) {
    indices.push_back(p0);
    indices.push_back(p1);
    indices.push_back(p2);
}

}
}

