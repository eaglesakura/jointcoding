/*
 * ColorUniform.hpp
 *
 *  Created on: 2013/04/20
 */

#ifndef COLORUNIFORM_HPP_
#define COLORUNIFORM_HPP_

#include    "jc/gl/shader/UniformBase.hpp"
#include    "jc/graphics/Color.h"

namespace jc {
namespace gl {

class ColorUniform: public UniformBase {
    /**
     * 転送済みの色
     */
    rgba32 color;
public:
    ColorUniform() {
        color = 0;
    }

    ~ColorUniform() {
    }

    /**
     * 色情報を転送する
     */
    jcboolean upload(const Color &c) {
        if (!valid()) {
            return jcfalse;
        }

        if (color != c.rgba) {
            glUniform4f(location, c.rf(), c.gf(), c.bf(), c.af());
            assert_gl();
            color = c.rgba;
            return jctrue;
        }
        return jcfalse;
    }

    /**
     * 色情報を転送する
     */
    jcboolean upload(const rgba32 c) {
        return upload(Color(c));
    }
};

}
}

#endif /* COLORUNIFORM_HPP_ */
