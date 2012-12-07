/*
 * JCGLObject.h
 *
 *  Created on: 2012/06/25
 *      Author: Takeshi
 */

#ifndef JCGLOBJECT_H_
#define JCGLOBJECT_H_

#include "JCGL.h"
#include "JCGLState.h"


namespace jc {
namespace gl {

/**
 * GLと関連づいたマネージドオブジェクトを定義する。
 * オブジェクトは必ずGLStateと関連付けられて管理される。
 * GLStateはcontextごとに一つ生成される。
 */
class GLObject: public Object {
protected:
    MGLState state;
    GLObject(const MGLState state);
public:
    virtual ~GLObject() {

    }

    /**
     * 関連付けられたステートを取得する。
     * 関連付けられたステートは基本的に不変である。
     */
    virtual MGLState getState() const {
        return state;
    }
};

}
}
#endif /* JCGLOBJECT_H_ */
