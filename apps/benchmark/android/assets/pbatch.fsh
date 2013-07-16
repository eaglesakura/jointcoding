/**
 * プリミティブバッチ処理用フラグメントシェーダー
 */
#version 100

#define MAX_TEXTURE_UNITS 16

/**
 * varyngs
 */
// UV
varying mediump vec2 vCoord;

// frag color
varying lowp vec4 vColor;

// Texture Unit
varying lowp float vTextureIndex;

/** 
 * uniforms
 */
uniform sampler2D uTextures[MAX_TEXTURE_UNITS];

lowp vec4 get_texcolor() {
    lowp int index = int(vTextureIndex);
    if (index < 0) {
        return vec4(1.0, 1.0, 1.0, 1.0);
    } else {
        return texture2D(uTextures[0], vCoord);
    }
}

/**
 * main
 */
void main() {

    // ピクセルカラーとテクスチャカラーをブレンドして最終色を出す
//    gl_FragColor = vColor * get_texcolor();
    gl_FragColor = vColor;
//    gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
