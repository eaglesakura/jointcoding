/*
 * GLFigure.cpp
 *
 *  Created on: 2012/12/26
 */
#include    "jc/gl/GLFigure.h"

namespace jc {
namespace gl {

GLFigure::GLFigure() {

}

GLFigure::~GLFigure() {

}

/**
 * ノードのレンダリングを行う
 */
void GLFigure::onNodeRendering(const s32 nodeNumber, FigureNode *node, const GLFigure::ShaderParams *params) {

    MGLState state = device->getState();
    const u32 material_num = node->renderingFragments.size();
    for (u32 mtl = 0; mtl < material_num; ++mtl) {
        MGLFigureMeshFragment fragment = node->renderingFragments[mtl];
        jc_sp<GLFigureMaterial> material = fragment->getMaterial();
        const u32 context_num = fragment->getDrawingContextCount();
        for (u32 ctx = 0; ctx < context_num; ++ctx) {
            GLFigureMeshFragment::DrawingContext *pContext = fragment->getDrawingContext(ctx).get();

            pContext->vertices->bind();
            pContext->indices->bind();

            // position
            {
                const u32 attr_pos = params->attributes.position;
                state->enableVertexAttribArray(attr_pos);
                state->vertexAttribPointer(attr_pos, 3, GL_FLOAT, GL_FALSE, sizeof(GLFigureVertex), NULL, 0);
            }

            // uv
            if (params->attributes.uv != ATTRIBUTE_DISABLE_INDEX) {
                const u32 attr_uv = params->attributes.uv;
                state->enableVertexAttribArray(attr_uv);
                state->vertexAttribPointer(attr_uv, 2, GL_FLOAT, GL_FALSE, sizeof(GLFigureVertex), NULL, sizeof(Vector3f));
            }

            // weight
            if (params->attributes.weight != ATTRIBUTE_DISABLE_INDEX

//                    && params->attributes.weight_indeices != ATTRIBUTE_DISABLE_INDEX
            ) {
                const u32 attr_weight = params->attributes.weight;
                const u32 attr_weight_indices = params->attributes.weight_indeices;

                state->enableVertexAttribArray(attr_weight);
                state->enableVertexAttribArray(attr_weight_indices);

                state->vertexAttribPointer(attr_weight_indices, 4, GL_FLOAT, GL_FALSE, sizeof(GLFigureVertex), NULL, sizeof(Vector3f) + sizeof(Vector2f));
                state->vertexAttribPointer(attr_weight, 4, GL_FLOAT, GL_FALSE, sizeof(GLFigureVertex), NULL, sizeof(Vector3f) + sizeof(Vector2f) + sizeof(Vector4f));
            }

            // texture
            if (params->uniforms.tex_0 != UNIFORM_DISABLE_INDEX && material->use_texture) {
                const u32 unif_tex0 = params->uniforms.tex_0;

                if (!material->texture) {
                    material->texture = textures->get(material->textureName);

                    // テクスチャが設定済みの場合、バインドする
                    if (material->texture) {
                        const s32 index = material->texture->bind();
                        glUniform1i(unif_tex0, index);
                    }
                } else {
                    // テクスチャが設定済みの場合、バインドする
                    if (material->texture) {
                        const s32 index = material->texture->bind();
                        glUniform1i(unif_tex0, index);
                    }
                }

            }

            // rendering!
            pContext->indices->rendering();
        }
    }
}

/**
 * 指定した番号のノードを描画・探索する
 */
void GLFigure::_rendering(const u32 nodeNumber, const GLFigure::ShaderParams *params) {
    FigureNode *node = getNodePtr(nodeNumber);
    onNodeRendering(nodeNumber, node, params);

    const u32 children_num = node->children.size();

    // 子ノードも順番に探索していく
    for (u32 i = 0; i < children_num; ++i) {
        _rendering(node->children[i], params);
    }
}

/**
 * レンダリングを開始する
 */
void GLFigure::rendering(const GLFigure::ShaderParams *params) {
    // 最初のレンダリングは0から、順に階層を追っていく
    _rendering(0, params);
}

/**
 * 各ノードの逆行列を作成する.
 * 初回に一度だけ呼び出せばいい。
 *
 * 各頂点はglobal行列適用済みのため、一度逆行列を通してローカルに落とし込んだ後、再度現在のボーンに合わせて行列を当てる必要がある。
 */
void GLFigure::initializeInvertMatrices() {
    _initializeInvertMatrices(0, Matrix4x4());
}
/**
 * 逆行列を作成する
 */
void GLFigure::_initializeInvertMatrices(const u32 nodeNumber, Matrix4x4 parent) {
    FigureNode *node = getNodePtr(nodeNumber);

    // make local
    Matrix4x4 local;
    node->defTransform.getMatrix(&local);

    // make global
    // 現在の行列 = デフォルトのワールド行列とする
    multiply(local, parent, &node->matrix_current_world);

    // invert!!
    node->matrix_current_world.invert(&node->matrix_default_invert);

    // 子を作成する
    for (u32 i = 0; i < node->children.size(); ++i) {
        _initializeInvertMatrices(node->children[i], node->matrix_current_world);
    }
}

}
}
