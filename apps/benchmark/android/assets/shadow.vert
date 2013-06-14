#version 100

// pos
attribute mediump vec4 attr_pos;

// blended world & look & projection
uniform highp mat4 unif_wlp;

void main() {
    // position
    gl_Position = unif_wlp * attr_pos;
}

