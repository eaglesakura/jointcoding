// pos
attribute mediump vec4 attr_pos;
// UV
attribute mediump vec2 attr_uv;
// normal
attribute mediump vec3 attr_normal;

// blended world & look & projection
uniform highp mat4 unif_wlp;

// export coord
varying mediump vec2 vr_coords;
//  export normal
varying mediump vec3 vr_normal;

void main() {
    // position
    gl_Position = unif_wlp * attr_pos;
    
    // normal
    vr_normal = (attr_normal / 2.0) + vec3(0.5, 0.5, 0.5);
    
    // uv
    vr_coords = attr_uv;
}
