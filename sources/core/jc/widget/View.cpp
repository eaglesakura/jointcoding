/*
 * View.cpp
 *
 *  Created on: 2013/04/11
 */

#include    "jc/widget/View.h"

namespace jc {
namespace view {

View::View() {
    this->focus = this->focusable = this->touchable = jcfalse;
}

View::~View() {

}

/**
 * レイアウトを更新する。
 */
void View::layout(const RectF &area) {
    if (area != localArea) {
        this->localArea = area;
        onLayoutChanged(area);
    }
}


}
}
