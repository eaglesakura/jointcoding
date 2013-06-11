/*
 * Light.hpp
 *
 *  Created on: 2013/06/01
 */

#ifndef LIGHT_HPP_
#define LIGHT_HPP_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

/**
 * ライトの種類を定義する
 */
enum LightType_e {
    /**
     * 方向ライト
     *
     * シャドウマップ生成や基本的なシェーディングで利用する
     */
    LightType_Direction,

    /**
     * 環境光
     */
    LightType_Ambient,
};

/**
 * レンダリング用ライトを定義する
 *
 * デフォルトは方向ライトが指定されている
 */
class Light: public Object {
    /**
     * ライトの種類
     */
    LightType_e type;
protected:
    /**
     * 方向を設定する
     * 真下を向かせるライト（蛍光灯等）は(0, -1, 0)となる
     */
    Vector3f direction;

    /**
     * ライト位置
     */
    Vector3f position;

    /**
     * ライトの色
     */
    Color color;
public:
    Light() {
        type = LightType_Direction;
        direction.set(0, -1, 0);
        position.set(0, 1, 0);
        color.RGBAi(0, 0, 0, 255);
    }

    virtual ~Light() {
    }

    /**
     * ライトタイプを設定する
     */
    virtual void setType(const LightType_e type) {
        this->type = type;
    }

    /**
     * 方向を取得する
     */
    const Vector3f& getDirection() const {
        return direction;
    }

    /**
     * ライトの方向を指定する
     * 指定された方向は必ず正規化される。
     */
    virtual void setDirection(const Vector3f &dir) {
        direction = dir;
        direction.normalize();
    }

    /**
     * posからlookを向けた場合の方向を指定する
     * シャドウマップ用行列を設定する場合、シャドウを入れたい範囲が映り込むようにposを指定する
     */
    virtual void setDirection(const Vector3f &pos, const Vector3f &look) {
        position = pos;
        setDirection(look - pos);
    }

    /**
     * ライト色を取得する
     */
    Color getColor() const {
        return color;
    }

    void setColor(const Color color) {
        this->color = color;
    }

};

/**
 * シャドウマップ用ライト
 */
class ShadowLight: public Light {
    /**
     * シャドウ制御用のカメラ
     */
    MCamera shadowCamera;
public:
    ShadowLight() {
        shadowCamera.reset(new Camera());
        shadowCamera->setPosition(position);
        shadowCamera->setLook(position + direction);
    }

    virtual ~ShadowLight() {
    }

    /**
     * ライトの方向を指定する
     * 指定された方向は必ず正規化される。
     */
    virtual void setDirection(const Vector3f &dir) {
        Light::setDirection(dir);
        shadowCamera->setLook(position + dir);

    }

    /**
     * posからlookを向けた場合の方向を指定する
     * シャドウマップ用行列を設定する場合、シャドウを入れたい範囲が映り込むようにposを指定する
     */
    virtual void setDirection(const Vector3f &pos, const Vector3f &look) {
        Light::setDirection(pos, look);
        shadowCamera->setPosition(pos);
    }

    /**
     * シャドウ制御用カメラを設定する
     */
    virtual MCamera getShadowCamera() const {
        return shadowCamera;
    }
}
;

/**
 * managed
 */
typedef jc_sp<Light> MLight;
/**
 * managed
 */
typedef jc_sp<ShadowLight> MShadowLight;

}
}

#endif /* LIGHT_HPP_ */
