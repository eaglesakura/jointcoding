#version 100

// pos
attribute mediump vec4 attr_pos;

// blended look & projection
uniform highp mat4 unif_lp;

// world
uniform highp mat4 unif_world;

void main() {
    // position
    gl_Position = unif_lp * unif_world * attr_pos;
}

