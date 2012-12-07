/*
 * ndkTestSupport.h
 *
 *  Created on: 2012/07/06
 */

#ifndef NDKTESTSUPPORT_H_
#define NDKTESTSUPPORT_H_

#include    "jcTestSupport.h"
#include    "jointcoding-android.h"
#include    "jcAssertError.h"

namespace jc {

class TestSupport {
public:
    /**
     * テスト用のメソッドを呼び出す。
     */
    static jcboolean callTestFnction(const charactor* packageName, const charactor* libName, const charactor* functionName);
};

}

#endif /* NDKTESTSUPPORT_H_ */
