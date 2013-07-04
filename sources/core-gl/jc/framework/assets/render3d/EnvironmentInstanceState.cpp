/*
 * EnvironmentInstanceState.cpp
 *
 *  Created on: 2013/06/02
 */

#include    "jc/framework/assets/Rendering3DAsset.hpp"

namespace jc {
namespace fw {

EnvironmentInstanceState::EnvironmentInstanceState() {
    mainCamera.reset(new Camera());
    shadowmapLight.reset(new ShadowLight());
}

EnvironmentInstanceState::~EnvironmentInstanceState() {
}

/**
 * WLP行列を計算する
 */
Matrix4x4* EnvironmentInstanceState::calcWorldLoopProjection(const Matrix4x4 &modelview, Matrix4x4 *pResult) const {
    multiply(modelview, mainCamera->getLookProjectionMatrix(), pResult);
    return pResult;
}

/**
 * シャドウマップ用WLP行列を計算する
 */
Matrix4x4* EnvironmentInstanceState::calcShadowWorldLoopProjection(const Matrix4x4 &modelview, Matrix4x4 *pResult) const {
    multiply(modelview, shadowmapLight->getShadowCamera()->getLookProjectionMatrix(), pResult);
    return pResult;
}

}
}

