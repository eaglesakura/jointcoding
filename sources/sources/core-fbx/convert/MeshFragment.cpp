/*
 * MeshFragment.cpp
 *
 *  Created on: 2012/12/18
 */

#include    "jcfbx/convert/MeshFragmentConverter.h"

namespace jc {
namespace fbx {

/**
 *
 */
MeshFragmentConverter::MeshFragmentConverter() {
    contexts.push_back(MFragmentContext(new FragmentContext()));
    // 最大で利用できるボーン数
    maxBones = 32;
}

/**
 *
 */
MeshFragmentConverter::~MeshFragmentConverter() {

}

/**
 * 利用できるボーン数をオーバーする場合true
 */
jcboolean MeshFragmentConverter::isOverBone(const u8 *bones, const s32 bones_length) {
    std::map<u8, u8> temp;

    // ダブリを防止する
    for (int i = 0; i < bones_length; ++i) {
        temp.insert(std::map<u8, u8>::value_type(bones[i], bones[i]));
    }

    int newBones = 0;
    std::map<u8, u8>::iterator itr = temp.begin(), end = temp.end();

    while (itr != end) {
        if (itr->first != UNUSED_BONE) {
            if (!isUsingBone(itr->first)) {
                ++newBones;
            }
        }

        ++itr;
    }

    FragmentContext *pContext = getCurrentContext();
    return ((int) pContext->useBoneIndices.size() + newBones) > maxBones;
}

/**
 * 既にボーンを使用済みか
 */
jcboolean MeshFragmentConverter::isUsingBone(u8 boneIndex) {
    FragmentContext *pContext = getCurrentContext();
    std::map<u8, u8>::iterator itr = pContext->useBoneIndices.begin(), end = pContext->useBoneIndices.end();

    while (itr != end) {

        if (itr->first == boneIndex) {
            return jctrue;
        }

        ++itr;
    }

    return jcfalse;
}

/**
 * インデックスバッファを追加する
 */
void MeshFragmentConverter::addIndices(const std::vector<FigureVertex> &vertices, const u16 p0, const u16 p1, const u16 p2) {
    int ptr = 0;
    u8 bones[4 * 3] = { };

    // 利用するボーンを列挙する
    {
        const FigureVertex &vertex = vertices[p0];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }
    {
        const FigureVertex &vertex = vertices[p1];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }
    {
        const FigureVertex &vertex = vertices[p2];
        for (int i = 0; i < 4; ++i) {
            bones[ptr] = vertex.weight.indices[i];
            ++ptr;
        }
    }

    if (isOverBone(bones, 12)) {
        // 子を分離させる
        separation();
    }

    // 頂点を登録する
    FragmentContext *pContext = getCurrentContext();
    {
        const FigureVertex &vertex = vertices[p0];
        pContext->indices.push_back(registerVertex(&pContext->vertices, vertex));
    }
    {
        const FigureVertex &vertex = vertices[p1];
        pContext->indices.push_back(registerVertex(&pContext->vertices, vertex));
    }
    {
        const FigureVertex &vertex = vertices[p2];
        pContext->indices.push_back(registerVertex(&pContext->vertices, vertex));
    }

    for (int i = 0; i < 12; ++i) {
        if (bones[i] != UNUSED_BONE) {
            pContext->useBoneIndices.insert(std::map<u8, u8>::value_type(bones[i], bones[i]));
        }
    }
}

/**
 * 直接的なボーンインデックスを、ピックアップテーブルのインデックスに変換する
 */
static u8 boneIndex2pickIndex(u8 *pBonePickTable, const u8 boneIndex) {
    if (boneIndex == (u8) UNUSED_BONE) {
        return (u8) 0;
    }

    u8 index = 0;
    while (true) {
        if ((*pBonePickTable) == boneIndex) {
//            jclogf("    pickup(%d -> %d)", boneIndex, index);
            return index;
        }

        ++index;
        ++pBonePickTable;
    }

    // 通常、ここまで来ることはない
    throw create_exception(RuntimeException, "Error!!");
}

/**
 * 共通のFigureFragmentに変換する
 */
MFigureMeshFragment MeshFragmentConverter::createFigureFragment() const {
    MFigureMeshFragment result(new FigureMeshFragment());

    for (int i = 0; i < (int) contexts.size(); ++i) {
        MFragmentContext figureContext = contexts[i];

        jc_sp<FigureMeshFragment::DrawingContext > context(new FigureMeshFragment::DrawingContext());

        // make weight
        {
            const s32 bonePickTable_length = figureContext->useBoneIndices.size();
            u8 *pBonePickTable = new u8[bonePickTable_length];

            int ptr = 0;
            std::map<u8, u8>::iterator itr = figureContext->useBoneIndices.begin(), end = figureContext->useBoneIndices.end();

            jclog("      ----- fragment begin -----");
            while (itr != end) {
                jclogf("      bone pick table[%d] = bone index %d", ptr, itr->first);
                pBonePickTable[ptr++] = itr->first;
                ++itr;
            }
            jclog("      ----- fragment begin -----");

            context->bone_pick_table.reset(pBonePickTable);
            context->bone_pick_table_length = bonePickTable_length;
        }

        // make vertices
        {
            const s32 vertices_length = figureContext->vertices.size();
            FigureVertex *pVertices = new FigureVertex[vertices_length];

            // copy
            for (int k = 0; k < vertices_length; ++k) {
                pVertices[k] = figureContext->vertices[k];

                // bone index -> pick index
                for (int weight_index = 0; weight_index < SIMPLE_BONE_NUM; ++weight_index) {
                    pVertices[k].weight.indices[weight_index] = boneIndex2pickIndex(context->bone_pick_table.get(), pVertices[k].weight.indices[weight_index]);
                }
            }

            context->vertices_length = vertices_length;
            context->vertices.reset(pVertices);
        }
        // make indices
        {
            const s32 indices_length = figureContext->indices.size();
            u16 *pIndices = new u16[indices_length];

            // copy
            for (int k = 0; k < indices_length; ++k) {
                pIndices[i] = figureContext->indices[k];
            }

            context->indices_length = indices_length;
            context->indices.reset(pIndices);
        }

        result->addDrawingContext(context);
    }
    return result;

}

/**
 * 現在のデータから分離を行い、新たなフラグメントとして子を作成する。
 */
void MeshFragmentConverter::separation() {

    MFragmentContext nextContext(new FragmentContext());
    contexts.push_back(nextContext);
}
/**
 * FBX Mesh -> JointCoding Meshに変換を行う
 */
void MeshFragmentConverter::convertMesh(std::vector<MFigureMeshFragment> *result, const VertexContainer &vertices, const IndicesContainer &indices) {

    /**
     *
     * 最適化した頂点リスト
     */
    std::vector<FigureVertex> compact_vertices;

    /**
     * 最適化したインデックスリスト
     */
    std::vector<u32> compact_indices;

    /**
     * 一時的に格納しておくリスト
     * 0 polyは描画の必要がないから登録しない
     */
    std::vector<MMeshFragmentConverter> converters;

// マテリアル数だけフラグメントを構築する
    for (int material_index = 0; material_index < (int) indices.materials.size(); ++material_index) {
        converters.push_back(MMeshFragmentConverter(new MeshFragmentConverter()));
    }

// すべての頂点を結合して、最適化する
    {
        const int polygon_num = (int) indices.polygons.size();
        for (int polygon_index = 0; polygon_index < polygon_num; ++polygon_index) {
            const ConvertedPolygon &polygon = indices.polygons[polygon_index];

            for (int k = 0; k < 3; ++k) {
                FigureVertex v;
                v.position = vertices.positions[polygon.position[k]];
                if (vertices.weights.size()) {
                    v.weight = vertices.weights[polygon.position[k]];
                }

                if (vertices.coords.size()) {
                    v.uv = vertices.coords[polygon.attributes[k]];
                }
                if (vertices.normals.size()) {
                    v.normal = vertices.normals[polygon.attributes[k]];
                }
                compact_indices.push_back(registerVertex(&compact_vertices, v));
            }
        }
    }

// 描画命令ごとにフラグメント化する
// 一度に描画可能なボーン数(uniform vector)には制限があるため、適度な個数で分割する
    {
        int current = 0;
        const int polygon_num = (int) indices.polygons.size();

        for (int polygon_index = 0; polygon_index < polygon_num; ++polygon_index) {
            const ConvertedPolygon &polygon = indices.polygons[polygon_index];

            // マテリアルごとに登録するFragmentを拾う
            MMeshFragmentConverter targetFragment = converters[polygon.material];

            // ポリゴンを登録する
            targetFragment->addIndices(compact_vertices, compact_indices[current + 0], compact_indices[current + 1], compact_indices[current + 2]);

            // カレントを進める
            current += 3;
        }
    }

// ポリゴンが存在するフラグメントだけを返す
    {
        std::vector<MMeshFragmentConverter>::iterator itr = converters.begin(), end = converters.end();

        int material_index = 0;
        while (itr != end) {
            if ((*itr)->getPolygonCount()) {

                MFigureMeshFragment fragment = (*itr)->createFigureFragment();
                fragment->setMaterial(indices.materials[material_index]);
                result->push_back(fragment);
            }
            ++material_index;
            ++itr;
        }
    }

    jclogf("    converted vertices(%d fragments)", result->size());
    for (int i = 0; i < (int) result->size(); ++i) {
        jclogf("      material[%s] fragments(%d)", (*result)[i]->getMaterial()->name.c_str(), (*result)[i]->getDrawingContextCount());
        for (int k = 0; k < (*result)[i]->getDrawingContextCount(); ++k) {
//            jclogf("        vertices(%d) poly(%d) bone(%d)", (*result)[i]->getVerticesCount(k), (*result)[i]->getPolygonCount(k), (*result)[i]->getBoneCount(k));
        }
    }
}

}
}

