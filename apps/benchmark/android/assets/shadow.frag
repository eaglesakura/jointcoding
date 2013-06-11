#version 100

// シャドウ行列を適用したポリゴン位置
varying mediump vec4 vr_shadowfrag;

void main() {
    gl_FragColor = vec4(vr_shadowfrag.z, 0.0, 0.0, 1.0);
//    gl_FragColor = vec4(gl_FragCoord.z / gl_FragCoord.w, 0.0, 0.0, 1.0);
}
