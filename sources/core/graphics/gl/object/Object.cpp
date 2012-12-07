/*
 * GLObject.cpp
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#include "JCGLObject.h"

namespace jc {
namespace gl {

GLObject::GLObject(const MGLState state) {
    if (state) {
        this->state = state;
    } else {
        this->state.reset(new GLState());
    }
}

}
}
