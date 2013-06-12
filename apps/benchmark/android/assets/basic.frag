#version 100

// texture
uniform sampler2D unif_diffuse;

// shadow texture
uniform sampler2D unif_shadowmap;

// shadow light
uniform mediump vec3 unif_mainlightdir;

//  normal
varying mediump vec3 vr_normal;

//  coord
varying mediump vec2 vr_coords;

// ライトと頂点法線の内積
varying mediump float vr_lightdot;

// シャドウ行列を適用したポリゴン位置
varying mediump vec4 vr_shadowfrag;


void main() {
    // テクスチャ色とライト係数で色を求める
    mediump vec4 light = vec4(vr_lightdot, vr_lightdot, vr_lightdot, 1.0);
    light *= vec4(1.0, 1.0, 1.0, 1.0);
    
    gl_FragColor = texture2D(unif_diffuse, vr_coords) * light;
    {
//        gl_FragColor = texture2D(unif_shadowmap, vr_shadowfrag.xy);
        // ライトからの距離を取り出す
        mediump float light_distance = texture2D(unif_shadowmap, vr_shadowfrag.xy).x + 0.005;
        // 陰の濃さ係数を計算し、濃さに合わせて陰を強くする
        // ライトからの距離とフラグメントの距離が近いほど、濃い陰が作られる
        mediump float shadow_level = vr_shadowfrag.z - light_distance;
        shadow_level = clamp(shadow_level * 2.0, 0.0, 1.0);
        gl_FragColor *= (1.0 - shadow_level);
        gl_FragColor.a = 1.0;
//        gl_FragColor = vec4(vec3(vr_shadowfrag.z), 1.0);
//        gl_FragColor = vec4(vec3(light_distance), 1.0);
//        if(vr_shadowfrag.z <= 0.5) {
//            gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//        }
//        if(light_distance > 0.0) {
//            gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//        }
    }
}
