/*
 * JCGLResource.h
 *
 *  Created on: 2012/06/27
 *      Author: Takeshi
 */

#ifndef JCGLRESOURCE_H_
#define JCGLRESOURCE_H_

#include    "JCGLObject.h"

namespace jc {
namespace gl {

/**
 * GLのリソースを管理するオブジェクト
 */
class Resource: public GLObject {
protected:
    Resource(const MGLState state);
public:
    virtual ~Resource();

    /**
     * 管理しているリソースを明示的に廃棄する
     */
    virtual void dispose() = 0;
};

}
}

#endif /* JCGLRESOURCE_H_ */
