#version 100

// pos
attribute mediump vec4 attr_pos;
// UV
attribute mediump vec2 attr_uv;

// vtx weight indices
attribute lowp vec4 attr_weight_indices;

// vtx weight
attribute lowp vec4 attr_weight;

// blended look & projection
uniform highp mat4 unif_look_prj;

// world
uniform highp mat4 unif_world;

// bone matrix table
uniform highp mat4 unif_bones[28];

// offset
uniform mediump vec2 unif_uv_offset;

// export coord
varying mediump vec2 vr_coords;

#define MAX_VERTEX_WEIGHT 4.0

void main() {
    highp vec4 n_pos = vec4(0, 0, 0, 0);
    mediump vec4 tmp_weight_indices = attr_weight_indices;
    mediump vec4 tmp_weight = attr_weight;
    // blend pos
    for( float i = 0.0; i < MAX_VERTEX_WEIGHT; i += 1.0) {
        highp vec4 blended = ( unif_bones[ int( tmp_weight_indices.x ) ] * attr_pos );
        blended.xyz *= tmp_weight.x;
        n_pos += blended;
        // vector shift
        // vec4[n]アクセスは拡張だから使えないGPUがある
        tmp_weight_indices = tmp_weight_indices.yzwx;
        tmp_weight = tmp_weight.yzwx;
    }
    n_pos.w = 1.0;
    // exp pos
    gl_Position = unif_look_prj * unif_world * n_pos;
    // uv
    vr_coords = attr_uv + unif_uv_offset;
}
