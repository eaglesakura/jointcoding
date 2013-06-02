#version 100

// texture
uniform sampler2D unif_diffuse;

// shadow light
uniform mediump vec3 unif_mainlightdir;

//  normal
varying mediump vec3 vr_normal;

//  coord
varying mediump vec2 vr_coords;

// ライトと頂点法線の内積
varying mediump float vr_lightdot;

void main() {
    // テクスチャ色とライト係数で色を求める
    mediump vec4 light = vec4(vr_lightdot, vr_lightdot, vr_lightdot, 1.0);
    light *= vec4(1.0, 0.75, 0.75, 1.0);
    gl_FragColor = texture2D(unif_diffuse, vr_coords) * light;
//    gl_FragColor.r = gl_FragCoord.z;
}
