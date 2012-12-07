/*
 * ImplCapsuleTest.cpp
 *
 *  Created on: 2012/08/20
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "jcImplCapsule.h"

#include    <string>

using namespace jc;

extern "C" {

TESTFUNCTION void ImplCapsule_makeTest() {
    ImplCapsule capsule2;
    {
        impl_ref ref = new std::string();
        jclogf("make ref : %x", ref);
        ImplCapsule capsule(ref, JC_CAPSULE_RELEAE(std::string));
        capsule2 = capsule;
    }
    {
        impl_ref ref = new char;
        jclogf("make ref : %x", ref);
        ImplCapsule capsule(ref, JC_CAPSULE_RELEAE(char));
    }
}

}

