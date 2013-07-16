/**
 * プリミティブバッチ処理用フラグメントシェーダー
 * 2Dスプライト処理用の精度で実装している
 */
#version 100

/**
 * attributes
 */
// 位置情報
// 2Dで指定する
attribute mediump vec4 aPosition;
// UV
attribute mediump vec2 aCoord;
// 頂点カラー
attribute lowp vec4 aColor;
// 回転
attribute lowp float aRotate;
// テクスチャユニット番号
attribute lowp float aTextureIndex;

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

/**
 * main
 */
void main() {
    {
        gl_Position = aPosition;
    }

    // 頂点カラー情報は直接出力
    {
        vColor = aColor;
    }
    // UV/Texture情報は直接出力
    {
        vTextureIndex = aTextureIndex;
        vCoord = aCoord;
    }
}
