/*
 * StringTest.cpp
 *
 *  Created on: 2012/12/25
 */

#include    "jointcoding-testsupport.h"
#include    "jc/system/jcString.h"
#include    "jc/system/StringUtil.h"
#include    <map>

using namespace jc;

TESTFUNCTION void test_string_hashMap() {
    std::map<String, int> temp;

    temp.insert(std::map<String, int>::value_type("0", 100));
    _assertEquals(temp.size(), 1);

    temp.insert(std::map<String, int>::value_type("0", 100));
    _assertEquals(temp.size(), 1);

    _assertEquals(temp.at("0"), 100);
}
