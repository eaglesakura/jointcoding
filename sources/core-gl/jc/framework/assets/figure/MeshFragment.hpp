/*
 * MeshFragment.hpp
 *
 *  Created on: 2013/05/21
 */

#ifndef MESHFCONTEXT_HPP_
#define MESHFCONTEXT_HPP_

#include    "jc/framework/assets/FigureAsset.hpp"

namespace jc {
namespace fw {

typedef VertexBufferObject<void> FigureVertices;

/**
 * メッシュの断片を構築する
 *
 * MeshFragmentは1つ以上組み合わせて１つのマテリアル部位を構築し、それらを組み合わせて１つのメッシュを構築する。
 * スキニングのボーン数制限を回避するため、このクラスが描画の最小単位となる。
 */
class MeshFragment: public Object {
protected:
    struct Range {
        s32 header;
        s32 length;

        Range() {
            header = length = 0;
        }
    } range;

public:
    MeshFragment() {
    }

    virtual ~MeshFragment() {
        jclogf("delete MeshFragment(0x%x)", this);
    }

    /**
     * 初期化の必要があるならば適宜行う
     */
    virtual void initialize(MDevice device) {

    }

    /**
     * 描画用のインデックスバッファのレンジを設定する
     * インデックスバッファはFigure用に１つだけ生成されており、そのレンジを指定する形になる。
     */
    virtual void setIndicesRange(const s32 header, const s32 length) {
        range.header = header;
        range.length = length;
    }

    /**
     * 描画用インデックスバッファのレンジを取得する
     *
     * ポインタは常にどちらも!=NULLでなければならない。
     */
    virtual void getIndicesRange(s32 *result_header, s32 *result_length) const {
        assert(result_header);
        assert(result_length);

        *result_header = range.header;
        *result_length = range.length;
    }
};

}
}

#endif /* MESHFRAGMENT_HPP_ */
