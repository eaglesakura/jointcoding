/*
 * SmartFBXObject.h
 *
 *  Created on: 2012/12/14
 */

#ifndef SMARTFBXOBJECT_H_
#define SMARTFBXOBJECT_H_

#include    "fbxsdk.h"
#include    "jc/mem/SmartPtr.h"

namespace jc {
namespace fbx {

template<typename T>
void delete_fbx(T *p) {
    if (p) {
        jclogf("Destroy FBX(%x)", p);
        p->Destroy();
        p = NULL;
    }
}

template<typename T>
class SmartFBXObject {
    jc_sp<T> fbxObj;

public:
    SmartFBXObject(T *p = NULL) {
        reset(p);
    }

    SmartFBXObject(const SmartFBXObject<T> &cpy ) {
        this->fbxObj = cpy.fbxObj;
    }

    virtual ~SmartFBXObject() {
        fbxObj.reset();
    }

    void reset( T *p = NULL ) {
        this->fbxObj.reset(p, &delete_fbx<T>);
    }

    SmartFBXObject<T>& operator =( const SmartFBXObject<T> &cpy ) {
        this->fbxObj = cpy.fbxObj;
        return (*this);
    }

    T* get() {
        return fbxObj.get();
    }

    const T* get() const {
        return fbxObj.get();
    }

    bool operator==( const SmartFBXObject<T> &right ) {
        return this->fbxObj == right.fbxObj;
    }

    T* operator->() {
        return get();
    }

    const T* operator->() const {
        return get();
    }

    bool operator!=(const SmartFBXObject<T> &right) {
        return this->fbxObj != right.fbxObj;
    }
};

}

}

#endif /* SMARTFBXOBJECT_H_ */
