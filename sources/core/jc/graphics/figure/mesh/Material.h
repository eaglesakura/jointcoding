/*
 * Material.h
 *
 *  Created on: 2012/12/17
 */

#ifndef JCFBXMATERIAL_H_
#define JCFBXMATERIAL_H_

#include "jointcoding.h"
#include "jc/graphics/Color.h"

namespace jc {

/**
 * 属性情報
 */
class FigureMaterial {
public:
    /**
     * テクスチャ名
     */
    String textureName;

    /**
     *
     */
    jc::Color diffuse;

    /**
     *
     */
    jc::Color ambient;

    /**
     *
     */
    jc::Color emissive;

    /**
     * マテリアル名そのもの
     */
    String name;
};

typedef jc_sp<FigureMaterial> MFigureMaterial;

}

#endif /* MATERIAL_H_ */
