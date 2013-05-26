// Figure fragment shader
precision mediump float;

// texture
uniform sampler2D unif_tex;

//  coord
varying vec2 vr_coords;

void main() {
    gl_FragColor = texture2D(unif_tex, vr_coords);
}
