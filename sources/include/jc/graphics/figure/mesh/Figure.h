/*
 * figure.h
 *
 *  Created on: 2012/12/19
 */

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

}

