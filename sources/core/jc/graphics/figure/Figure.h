/*
 * FIgure.h
 *
 *  Created on: 2012/12/26
 */

#ifndef FIGURE_H_
#define FIGURE_H_

#include    "jc/graphics/figure/mesh/Material.h"
#include    "jc/graphics/figure/mesh/MeshFragment.h"
#include    "jc/graphics/figure/mesh/Vertex.h"

namespace jc {

/**
 * FbxFigureを構築するメッシュ情報
 */
typedef _MeshFragment<jc::fbx::FigureVertex, jc::FigureMaterial> FigureMeshFragment;

/**
 * managed
 */
typedef jc_sp<FigureMeshFragment> MFigureMeshFragment;


/**
 * フィギュアを構築するノードの種別
 */
enum NodeType_e {
    /**
     * NULLオブジェクト用
     * 例えば、視線ターゲットやIKターゲットに利用する
     */
    NodeType_NULL,

    /**
     * 通常メッシュ
     * スキニング情報も持っている
     */
    NodeType_Mesh,
};

}

#endif /* FIGURE_H_ */
