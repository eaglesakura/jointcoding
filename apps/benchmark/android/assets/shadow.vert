#version 100

// pos
attribute mediump vec4 attr_pos;

// blended world & look & projection
uniform highp mat4 unif_wlp;

// シャドウ行列を適用したポリゴン位置
varying mediump vec4 vr_shadowfrag;

void main() {
    // position
    gl_Position = unif_wlp * attr_pos;
    gl_Position = gl_Position / gl_Position.w;
    gl_Position.z = (gl_Position.z - 0.5) * 2.0;

    // シャドウ計算用
    vr_shadowfrag = unif_wlp * attr_pos;
    vr_shadowfrag = vr_shadowfrag / vr_shadowfrag.w;
}

