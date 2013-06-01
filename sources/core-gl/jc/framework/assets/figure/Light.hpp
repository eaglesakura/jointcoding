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

    /**
     * 方向を設定する
     */
    Vector3f direction;

    /**
     * ライトの色
     */
    Color color;
public:
    Light() {
        type = LightType_Direction;
        direction.set(0, -1, 0);
        color.RGBAi(0, 0, 0, 255);
    }

    virtual ~Light() {
    }

    /**
     * 方向を取得する
     */
    virtual const Vector3f& getDirection() const {
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
     */
    virtual void setDirection(const Vector3f &pos, const Vector3f &look) {
        direction = look - pos;
        direction.normalize();
    }

    virtual Color getColor() const {
        return color;
    }

    virtual void setColor(const Color color) {
        this->color = color;
    }
};

/**
 * managed
 */
typedef jc_sp<Light> MLight;

}
}

#endif /* LIGHT_HPP_ */
