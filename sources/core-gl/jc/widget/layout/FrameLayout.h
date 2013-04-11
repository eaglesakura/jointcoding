/*
 * FrameLayout.h
 *
 *  Created on: 2013/04/11
 */

#ifndef FRAMELAYOUT_H_
#define FRAMELAYOUT_H_

#include    "jc/widget/View.h"

namespace jc {
namespace view {

class FrameLayout: public Object {
public:
    FrameLayout() {
    }

    virtual ~FrameLayout() {
    }
};

/**
 * managed
 */
typedef jc_sp<FrameLayout> MFrameLayout;

}
}

#endif /* FRAMELAYOUT_H_ */
