// texture
uniform sampler2D unif_tex;

//  normal
varying mediump vec3 vr_normal;

//  coord
varying mediump vec2 vr_coords;


void main() {

    // 法線色で染める
//    gl_FragColor = texture2D(unif_tex, vr_coords);
    gl_FragColor = vec4(vr_normal.r, vr_normal.g, vr_normal.b, 1.0);
}
