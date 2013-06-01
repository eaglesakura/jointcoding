/*
 * FigureAsset.hpp
 *
 * フィギュアを簡単に扱えるようにカプセル化する
 *  Created on: 2013/05/20
 */

#ifndef FIGUREASSET_HPP_
#define FIGUREASSET_HPP_

#include    "jc/framework/assets/ImageAsset.hpp"

// カスタマイズした頂点やシェーダーを扱いたい場合は適宜拡張を行う
#include    "jc/framework/assets/figure/FigureVertex.hpp"

// メッシュ
#include    "jc/framework/assets/figure/MeshFragment.hpp"
#include    "jc/framework/assets/figure/MeshGroup.hpp"
#include    "jc/framework/assets/figure/MeshMaterial.hpp"
#include    "jc/framework/assets/figure/Node.h"
#include    "jc/framework/assets/figure/Figure.h"
#include    "jc/framework/assets/figure/FigureInstanceState.hpp"
#include    "jc/framework/assets/figure/FigureRenderer.hpp"
#include    "jc/framework/assets/figure/BasicFigureRenderer.h"

// Figure Loader
//#include    "jc/framework/assets/figure/FigureLoader.h"

#endif /* FIGUREASSET_HPP_ */
