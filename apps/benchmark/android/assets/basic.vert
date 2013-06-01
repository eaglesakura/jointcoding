#version 100

// pos
attribute mediump vec4 attr_pos;
// UV
attribute mediump vec2 attr_uv;
// normal
attribute mediump vec3 attr_normal;

// blended world & look & projection
uniform highp mat4 unif_wlp;

// 陰計算用の方向ライト
uniform mediump vec3 unif_mainlightdir;

// normal行列
uniform mediump mat3 unif_normalm;

//  export coord
varying mediump vec2 vr_coords;

//  export normal
varying mediump vec3 vr_normal;

// ライトと頂点法線の内積
varying mediump float vr_lightdot;

void main() {
    // position
    gl_Position = unif_wlp * attr_pos;
    
    // 法線・ライティング
    {
        // 変換済み法線値
        mediump vec3 trans_normal = normalize( unif_normalm * attr_normal );
        
        vr_normal = trans_normal;
        // ライト法線との内積を求める
        // 内積値が小さければライトと対面している = 明るい
        // 内積値が大きければライトに背を向けている = 暗い
        vr_lightdot = (-dot(unif_mainlightdir, vr_normal) + 1.0) / 2.0;
    }
    
    // uv
    vr_coords = attr_uv;
}
