/*
 * FigureDataOutputStream.cpp
 *
 *  Created on: 2012/12/25
 */

#include    "jcfbx/output/FigureDataOutputStream.h"

namespace jc {

FigureDataOutputStream::FigureDataOutputStream(MOutputStream os) :
        BinaryOutputStream(os) {
    this->stream = stream;
    this->fixedbits = FIXED_BIT_GL;
    modeler = ModelerType_Unknown;
}

FigureDataOutputStream::~FigureDataOutputStream() {

}

/**
 * モデラーを設定する
 */
void FigureDataOutputStream::initialize(const ModelerType_e modeler) {
    this->modeler = modeler;

    modelMatrix.identity();
    switch (modeler) {
        case ModelerType_3dsMax:
//            modelMatrix.rotate(1, 0, 0, 90);
            break;
        default:
            break;
    }
}

/**
 * 頂点を書き出す
 * モデラー補正を行う
 *
 * @param v 書き出す頂点座標
 * @param transform 頂点の位置を補正する場合true
 */
void FigureDataOutputStream::writeVector3(const Vector3f &v, const jcboolean transform) {
    Vector3f temp;

    // 事前モデル行列を割り当てる
    modelMatrix.multiply(v, &temp);

    this->writeFixed32Array((const float*) &v, 3, fixedbits);
}

/**
 * 頂点配列を直接吐き出す。
 * Vertex構造体を直接渡せるように、GLと同じく隙間を設定できる
 */
void FigureDataOutputStream::writeVertices(const void* vertices, const u32 vertices_length, const u32 sizeof_once_vertex, const jcboolean transform) {
    u8* current_ptr = (u8*) vertices;

    // 全頂点に対して適用する
    for (u32 i = 0; i < vertices_length; ++i) {
        writeVector3(*((Vector3f*) current_ptr), transform);
        current_ptr += sizeof_once_vertex;
    }
}

/**
 * UV座標を書き出す
 *
 */
void FigureDataOutputStream::writeCoord(const Vector2f &v) {
    this->writeFixed32Array((const float*) &v, 2, fixedbits);
}

}

