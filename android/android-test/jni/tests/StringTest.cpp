/*
 * ImplCapsuleTest.cpp
 *
 *  Created on: 2012/08/20
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "jcImplCapsule.h"

using namespace jc;

extern "C" {

void log(const String str) {
    jclogf("str = %s", str.c_str());
}

TESTFUNCTION void String_helloTest() {
    String a = "ふぁいなる";
    log(a);

    a+= "だだっだ";
    log(a);
}

}

