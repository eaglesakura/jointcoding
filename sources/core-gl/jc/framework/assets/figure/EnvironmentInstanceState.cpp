/*
 * EnvironmentInstanceState.cpp
 *
 *  Created on: 2013/06/02
 */

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

EnvironmentInstanceState::EnvironmentInstanceState() {
    mainCamera.reset(new Camera());
    shadowLight.reset(new Light());
}

EnvironmentInstanceState::~EnvironmentInstanceState() {
}

/**
 * 内部で管理している行列情報を更新する
 */
void EnvironmentInstanceState::updateMatrices() {
    // メインカメラ情報を更新する
    {

    }
}

/**
 * WLP行列を計算する
 */
Matrix4x4* EnvironmentInstanceState::calcWorldLoopProjection(const Matrix4x4 &modelview, Matrix4x4 *pResult) const {
    multiply(modelview, mainCamera->getLookProjectionMatrix(), pResult);
    return pResult;
}

}
}

