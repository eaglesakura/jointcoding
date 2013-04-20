/*
 * SpriteExternalImageShader.hpp
 *
 *  Created on: 2013/04/20
 */

#ifndef SPRITEEXTERNALIMAGESHADER_HPP_
#define SPRITEEXTERNALIMAGESHADER_HPP_

namespace {

static const jc::charactor *VERTEX_EXTERNAL_SHADER_SOURCE =
//
        /**
         * ( poly_x | poly_y )はそれぞれ0.0f〜1.0fの正規化座標で設定する。
         */
// ポリゴンのXYWH
        ""
                "uniform mediump vec4    poly_data;"
// ポリゴンの回転角度
                "uniform mediump float   aspect;"
                "uniform mediump float   rotate;"
// ポリゴンのUV情報
                "uniform mediump mat4    unif_texm;\n"

// アクセス用のショートカットを設定する
                "#define poly_x         poly_data.x\n"
                "#define poly_y         poly_data.y\n"
                "#define poly_width     poly_data.z\n"
                "#define poly_height    poly_data.w\n"
//
                "attribute vec4 vTexCoord;"
                "attribute vec4 vPosition;"
                "varying vec2 fTexCoord;"

                "void main() {"
// 位置操作
                "   mediump mat4 mat = mat4(1.0);"
                "   {"
                "       mediump mat4 temp = mat4(1.0);"

// 移動行列を作成する
                "       {"
                "           mat[3][0] = poly_x;"
                "           mat[3][1] = poly_y;"
                "       }"
// 回転行列を作成する
                "       {"
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
                "       {"
                "           temp = mat4(1.0);"
                "           temp[0][0] = 1.0 / aspect;"
                "           mat = mat * temp;"
                "       }"
                "       gl_Position = mat * vPosition;"
                "   }"
// テクスチャ操作
                "   {"
                "       fTexCoord.xy = (unif_texm * vTexCoord).xy;"
                "   }"
                "}"
//
;

static const jc::charactor *FRAGMENT_EXTERNAL_SHADER_SOURCE = ""
        ""
        "#extension GL_OES_EGL_image_external : require\n"
        "        precision mediump float;"
        // UV setting"
        "        varying vec2 fTexCoord;"
        // texture
        "        uniform samplerExternalOES tex;"
// color
        "        uniform mediump vec4    blendColor;"
        "        void main() {"
        "            gl_FragColor = texture2D(tex, fTexCoord) * blendColor;"
        "        }"
//
;

}
#endif /* SPRITEEXTERNALIMAGESHADER_HPP_ */
