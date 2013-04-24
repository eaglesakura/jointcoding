/*
 * LinearLayout.hpp
 *
 *  Created on: 2013/04/24
 */

#ifndef LINEARLAYOUT_HPP_
#define LINEARLAYOUT_HPP_

#include    "jc/widget/View.h"

namespace jc {
namespace view {

/**
 * レイアウトの配置方向
 */
enum LayoutDirection_e {
    /**
     * 縦方向に配置する
     */
    LayoutDirection_Vertical,

    /**
     * 横方向に配置する
     */
    LayoutDirection_Horizontal,
};

class LinearLayout: public View {
public:

};

}
}

#endif /* LINEARLAYOUT_HPP_ */
