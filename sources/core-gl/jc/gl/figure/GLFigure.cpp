/*
 * GLFigure.cpp
 *
 *  Created on: 2012/12/26
 */
#include    "jc/gl/figure/GLFigure.h"

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

    // マテリアル数だけ繰り返す
    for (u32 mtl = 0; mtl < material_num; ++mtl) {
        MGLFigureMeshFragment fragment = node->renderingFragments[mtl];
        jc_sp<GLFigureMaterial> material = fragment->getMaterial();
        const u32 context_num = fragment->getDrawingContextCount();

        // コンテキスト数だけ、同一マテリアルで描画する
        for (u32 ctx = 0; ctx < context_num; ++ctx) {
            GLFigureMeshFragment::DrawingContext *pContext = fragment->getDrawingContext(ctx).get();

            pContext->vertices->bind(device->getState());
            pContext->indices->bind(device->getState());

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
            // uv offset
            if (params->uniforms.uv_offset != UNIFORM_DISABLE_INDEX) {
                const u32 unif_uv_offset = params->uniforms.uv_offset;
                glUniform2f(unif_uv_offset, material->offset.u, material->offset.v);
            }

            // weight
            if (params->attributes.weight != ATTRIBUTE_DISABLE_INDEX && params->attributes.weight_indeices != ATTRIBUTE_DISABLE_INDEX && params->uniforms.bones != UNIFORM_DISABLE_INDEX) {
                const u32 attr_weight = params->attributes.weight;
                const u32 attr_weight_indices = params->attributes.weight_indeices;
                const u32 unif_bones = params->uniforms.bones;

                state->enableVertexAttribArray(attr_weight);
                state->enableVertexAttribArray(attr_weight_indices);

                state->vertexAttribPointer(attr_weight_indices, 4, GL_FLOAT, GL_FALSE, sizeof(GLFigureVertex), NULL, sizeof(Vector3f) + sizeof(Vector2f));
                state->vertexAttribPointer(attr_weight, 4, GL_FLOAT, GL_FALSE, sizeof(GLFigureVertex), NULL, sizeof(Vector3f) + sizeof(Vector2f) + sizeof(Vector4f));

                // ボーンテーブルを転送
                {
                    _enumBones(fragment.get(), pContext);
                    glUniformMatrix4fv(unif_bones, pContext->bone_pick_table_length, GL_FALSE, (float*) bone_table);
                }
            }

            // texture
            if (params->uniforms.tex_0 != UNIFORM_DISABLE_INDEX && material->use_texture) {
                const u32 unif_tex0 = params->uniforms.tex_0;
                if (!material->texture) {
                    material->texture = textures->get(material->textureName);

                    // テクスチャが設定済みの場合、バインドする
                    if (material->texture) {
                        const s32 index = material->texture->bind(device->getState());
                        glUniform1i(unif_tex0, index);
                    }
                } else {
                    // テクスチャが設定済みの場合、バインドする
                    if (material->texture) {
                        const s32 index = material->texture->bind(device->getState());
                        glUniform1i(unif_tex0, index);
                    }
                }
            }
            // rendering!
            pContext->indices->rendering(GL_TRIANGLES);
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
 * 指定したノード番号のボーンテーブルを用意する
 */
void GLFigure::_enumBones(GLFigureMeshFragment *pFragment, GLFigureMeshFragment::DrawingContext *pContext) {
    for (u32 i = 0; i < pContext->bone_pick_table_length; ++i) {
        const u32 node_nubmer = pContext->bone_pick_table[i];
        MFigureNode bone_node = nodes[node_nubmer];
        const Matrix4x4 &bone = bone_node->matrix_current_world;
        const Matrix4x4 &bone_inv = bone_node->matrix_default_invert;

        multiply(bone_inv, bone, &bone_table[i]);
    }
}
/**
 * アニメーションを当てる
 */
void GLFigure::_posing(AnimationClip *animation, const u32 nodeNumber, const Matrix4x4 &parent) {
    FigureNode *node = getNodePtr(nodeNumber);

    Matrix4x4 local;
    animation->getMatrix(nodeNumber, &local);
    multiply(local, parent, &node->matrix_current_world);

    // 子を設定する
    for (u32 i = 0; i < node->children.size(); ++i) {
        _posing(animation, node->children[i], node->matrix_current_world);
    }
}

/**
 * 逆行列を作成する
 */
void GLFigure::_initializeInvertMatrices(const u32 nodeNumber, const Matrix4x4 &parent) {
    FigureNode *node = getNodePtr(nodeNumber);
    Matrix4x4 local;
    node->defTransform.getMatrix(&local);
    multiply(local, parent, &node->matrix_current_world);

    // invert!!
    node->matrix_current_world.invert(&node->matrix_default_invert);

    // 子を作成する
    for (u32 i = 0; i < node->children.size(); ++i) {
        _initializeInvertMatrices(node->children[i], node->matrix_current_world);
    }
}

/**
 * アニメーション情報に合わせて、ポーズを取らせる。
 * 行列はAnimationClip側で計算するため、単純な行列置き換えのみを行う。
 */
void GLFigure::posing(AnimationClip *animation) {
    _posing(animation, 0, Matrix4x4());
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
 * レンダリング時のUV値オフセットを設定する
 */
void GLFigure::setUVOffset(const String &mat_name, const float u, const float v) {
    for (u32 k = 0; k < materials.size(); ++k) {
        MGLFigureMaterial material = materials[k];
        if (material->name == mat_name) {
            material->offset.u = u;
            material->offset.v = v;
        }
    }
}

/**
 * マテリアルを登録する。
 * 同一名・同一テクスチャのマテリアルがあれば、そのマテリアルを返してshareできるようにする。
 * 無い場合はキャッシュに新規登録し、oldをそのまま返す。
 */
MGLFigureMaterial GLFigure::putMaterial(const MGLFigureMaterial old) {
    // キャッシュを調べる
    {
        std::vector<MGLFigureMaterial>::iterator itr = materials.begin(), end = materials.end();
        while (itr != end) {
            MGLFigureMaterial cache = (*itr);
            if (cache->name == old->name && cache->textureName == old->textureName) {
                jclogf("    material cache name(%s) tex(%s)", cache->name.c_str(), cache->textureName.c_str());
                return cache;
            }
            ++itr;
        }
    }

    // キャッシュヒットしなかったから、oldを登録する
    materials.push_back(old);
    return old;
}

}
}
