/*
 * JCObject.h
 *
 *  Created on: 2012/05/16
 *      Author: Takeshi
 */

#ifndef JCOBJECT_H_
#define JCOBJECT_H_

#include    "jointcoding.h"

namespace jc {
/**
 * 全てのクラスのROOTクラス
 */
class Object {

public:
    Object() {
        jcmark(this);
    }

    virtual ~Object() {
        jcunmark(this);
    }
};

}
#endif /* JCOBJECT_H_ */
