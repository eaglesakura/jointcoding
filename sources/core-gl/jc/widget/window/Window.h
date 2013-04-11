/*
 * Window.h
 *
 *  Created on: 2013/04/11
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include    "jc/widget/View.h"

namespace jc {
namespace view {

/**
 * すべてのViewを入れるためのRootとして機能する
 */
class Window: public View {
public:
    Window(const MWindowContext windowContext) {
        this->windowContext = windowContext;
    }

    virtual ~Window() {
    }
};

/**
 * managed
 */
typedef jc_sp<Window> MWindow;

}
}

#endif /* WINDOW_H_ */
