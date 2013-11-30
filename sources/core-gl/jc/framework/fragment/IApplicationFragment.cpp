/*
 * IApplicationFragment.cpp
 *
 *  Created on: 2013/11/30
 */

#include    "jc/framework/fragment/IApplicationFragment.hpp"
#include    "jc/framework/fragment/ApplicationFragmentController.hpp"
#include    "jc/framework/app/JointApplicationBase.h"

namespace jc {
namespace gl {

/**
 * アタッチされているアプリを取得する
 */
JointApplicationBase* IApplicationFragment::getApplication() const {
    assert(controller);
    return controller->getApplication();
}

MDevice IApplicationFragment::getWindowDevice() const {
    return  getApplication()->getWindowDevice();
}


}
}
