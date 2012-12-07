/*
 * jcGLMaterial.h
 *
 *  Created on: 2012/08/21
 */

#ifndef JCGLMATERIAL_H_
#define JCGLMATERIAL_H_

#include    "jcGL.h"
#include    "jcGLShaderProgram.h"

namespace jc {
namespace gl {

/**
 * GL上で扱うマテリアルを抽象化する
 */
class GLMaterial: public Object {

public:
    GLMaterial();
    virtual ~GLMaterial();
};

}
}

#endif /* JCGLMATERIAL_H_ */
