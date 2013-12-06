/*
 * FigureRenderer.cpp
 *
 *  Created on: 2013/01/18
 */

#include    "jc/gl/figure/FigureRenderer.h"

namespace jc {
namespace gl {

FigureRenderer::FigureRenderer() {
    params.reset(mark_new GLFigure::ShaderParams());
    camera.reset(mark_new Camera());
    unif_look_prj = UNIFORM_DISABLE_INDEX;
    unif_world = UNIFORM_DISABLE_INDEX;
}

FigureRenderer::~FigureRenderer() {
}


/**
 * 初期化を行う
 */
void FigureRenderer::initialize( MDevice device, const Uri &vertexShader, const Uri &fragmentShader ) {
    shader = ShaderProgram::buildFromUri(device, vertexShader, fragmentShader);
    if (!shader) {
        throw create_exception(RuntimeException, "shader not found...");
    }

    // setup attributes
    params->attributes.position = shader->getAttribLocation("attr_pos");
    params->attributes.uv = shader->getAttribLocation("attr_uv");
    params->attributes.weight_indeices = shader->getAttribLocation("attr_weight_indices");
    params->attributes.weight = shader->getAttribLocation("attr_weight");
    params->uniforms.tex_0 = shader->getUniformLocation("unif_tex");
    params->uniforms.bones = shader->getUniformLocation("unif_bones");
    params->uniforms.uv_offset = shader->getUniformLocation("unif_uv_offset");

    unif_world = shader->getUniformLocation("unif_world");
    unif_look_prj = shader->getUniformLocation("unif_look_prj");
}


/**
 * 描画用のカメラをセットアップする
 */
void FigureRenderer::bindCamera(MGLState state) {
    shader->bind(state);

    Matrix4x4 m_camera;
    camera->getMatrix(&m_camera);

    glUniformMatrix4fv(unif_look_prj, 1, GL_FALSE, (float*) &m_camera);
}

}
}
