/*
 * SpriteImageShader.hpp
 *
 *  Created on: 2013/04/20
 */

#ifndef SPRITEIMAGESHADER_HPP_
#define SPRITEIMAGESHADER_HPP_

namespace {

static const jc::charactor *VERTEX_SHADER_SOURCE =
//
        /**
         * ( poly_x | poly_y )はそれぞれ0.0f〜1.0fの正規化座標で設定する。
         * poly_yについては2D座標系を基本として、画面左上が(0, 0)、画面右下が(1, 1)として扱う。
         */
// ポリゴンのXYWH
        ""
                "uniform mediump vec4    poly_data;"
                "uniform mediump float   aspect;"
// ポリゴンの回転角度
                "uniform mediump float   rotate;"
// ポリゴンのUV情報
                "uniform mediump vec4    poly_uv;\n"
// アクセス用のショートカットを設定する
                "#define poly_x         poly_data.x\n"
                "#define poly_y         poly_data.y\n"
                "#define poly_width     poly_data.z\n"
                "#define poly_height    poly_data.w\n"
//
                "#define poly_uv_u      poly_uv.x\n"
                "#define poly_uv_v      poly_uv.y\n"
                "#define poly_uv_w      poly_uv.z\n"
                "#define poly_uv_h      poly_uv.w\n"
//
                "attribute vec4 vTexCoord;"
                "attribute vec4 vPosition;"
                "varying vec2 fTexCoord;"

                "void main() {"
                "mediump mat4 mat = mat4(1.0);"
                "mediump mat4 temp = mat4(1.0);"
                "   {"

// 移動行列を作成する
                "       {"
                "           mat[3][0] = poly_x;"
                "           mat[3][1] = poly_y;"
                "       }"
// アスペクト比補正をかける
                "       {"
                "           temp[0][0] = 1.0 / aspect;"
                "           mat = mat * temp;"
                "       }"
// 回転行列を作成する
                "       {"
                "           temp = mat4(1.0);"
                "           temp[0][0] = cos(rotate);"
                "           temp[1][0] = sin(rotate);"
                "           temp[0][1] = -temp[1][0];"
                "           temp[1][1] = temp[0][0];"
                "           mat = mat * temp;"
                "       }"
// スケーリング行列を作成する
                "       {"
                "           temp = mat4(1.0);"
                "           temp[0][0] = poly_width * aspect;"
                "           temp[1][1] = poly_height;"
                "           mat = mat * temp;"
                "       }"
                "       gl_Position = mat * vPosition;"
                "   }"
// テクスチャ操作
                "   {"
                "       mat = mat4(1.0);"
                "       {"
// スケーリング行列を作成する
                "           mat[0][0] = poly_uv_w;"
                "           mat[1][1] = poly_uv_h;"
// 移動行列を作成する
                "           mat[3][0] = poly_uv_u;"
                "           mat[3][1] = poly_uv_v;"
                "       }"
                "       fTexCoord.xy = (mat * vTexCoord).xy;"
                "   }"
                "}";

static const jc::charactor *FRAGMENT_SHADER_SOURCE = ""
        ""
        "        precision mediump float;"
        // UV setting"
        "        varying vec2 fTexCoord;"
        // texture
        "        uniform sampler2D tex;"
        // color
        "        uniform mediump vec4    blendColor;"
        "        void main() {"
        "            gl_FragColor = texture2D(tex, fTexCoord) * blendColor;"
        "        }";

}
#endif /* SPRITEIMAGESHADER_HPP_ */
