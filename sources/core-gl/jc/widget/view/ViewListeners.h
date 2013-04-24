/*
 * ViewListeners.h
 *
 *  Created on: 2013/04/24
 */

#ifndef GLVIEW_VIEWLISTENERS_H_
#define GLVIEW_VIEWLISTENERS_H_

#include    "jointcoding.h"

namespace jc {
namespace view {

class View;
/**
 * Viewがクリックされたタイミングでコールバックされる
 */
class OnClickListener {
public:
    virtual ~OnClickListener() {
    }

    /**
     * クリックされた
     */
    virtual void onClick(jc_sp<View> view) = 0;
};

/**
 * selector
 */
typedef jc_selp<OnClickListener> SOnClickListener;

}
}

#endif /* VIEWLISTENERS_H_ */
