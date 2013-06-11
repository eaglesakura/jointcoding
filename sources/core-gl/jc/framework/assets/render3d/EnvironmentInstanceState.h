/*
 * EnvironmentInstanceState.hpp
 *
 *  Created on: 2013/06/01
 */

#ifndef ENVIRONMENTINSTANCESTATE_HPP_
#define ENVIRONMENTINSTANCESTATE_HPP_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * レンダリング用のワールド情報を格納する
 *
 * FigureInstanceStateは環境を持ち、環境によって現在のレンダリングを行う
 */
class EnvironmentInstanceState: public Object {
    /**
     * メインカメラ
     */
    MCamera mainCamera;

    /**
     * シャドウを計算するためのメインライト
     * 必ず方向ライトが設定されている。
     *
     * 複数光源によるシャドウをレンダリングしたい場合は適宜拡張を行うようにする。
     */
    MShadowLight shadowmapLight;

    /**
     * シャドウマップ計算用カメラ
     */
    MCamera shadowmapCamera;
public:
    EnvironmentInstanceState();
    virtual ~EnvironmentInstanceState();

    /**
     * 最終レンダリングに利用するディスプレイ用カメラを取得する
     */
    virtual MCamera getMainCamera() const {
        assert(mainCamera);
        return mainCamera;
    }

    /**
     * シャドウ計算用のメインライトを取得する
     */
    virtual MShadowLight getShadowmapLight() const {
        assert(shadowmapLight);
        return shadowmapLight;
    }

    /**
     * WLP行列を計算する
     */
    virtual Matrix4x4* calcWorldLoopProjection(const Matrix4x4 &modelview, Matrix4x4 *pResult) const;

    /**
     * シャドウマップ用WLP行列を計算する
     */
    virtual Matrix4x4* calcShadowWorldLoopProjection(const Matrix4x4 &modelview, Matrix4x4 *pResult) const;
};

/**
 * managed
 */
typedef jc_sp<EnvironmentInstanceState> MEnvironmentInstanceState;

}
}

#endif /* ENVIRONMENTINSTANCESTATE_HPP_ */
