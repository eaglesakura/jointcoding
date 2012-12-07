/*
 * jcGLMaterialFragment.h
 *
 *  Created on: 2012/08/21
 */

#ifndef JCGLMATERIALFRAGMENT_H_
#define JCGLMATERIALFRAGMENT_H_

#include    "jcMacro.h"
#include    "jcSmartPtr.h"
#include    "jcRenderPacket.h"

namespace jc {
namespace gl {

/**
 * マテリアル内の１要素を構築するクラス。
 * 例えばテクスチャグループやライト設定等
 */
class GLMaterialFragment: public Object, virtual public RenderPacketProtocol {
public:
    GLMaterialFragment() {
    }

    virtual ~GLMaterialFragment() {
    }
};

/**
 * 複数のテクスチャをグルーピングして扱うFragment
 */
class GLTextureGroupFragment: public GLMaterialFragment {
    struct Context {
        /**
         * 属性ID
         */
        GLuint attr_id;

        /**
         * テクスチャユニットのID
         */
        GLuint tex_unit_id;
    };

    /**
     *
     */
    jc_sa<Context> units;
};

}
}

#endif /* JCGLMATERIALFRAGMENT_H_ */
