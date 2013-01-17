/*
 * FileTest.cpp
 *
 *  Created on: 2013/01/17
 */

#include    "jointcoding.h"
#include    "jointcoding-testsupport.h"

#include    <dirent.h>

using namespace jc;

TESTFUNCTION void test_fileEnum() {
    DIR *dir = opendir("./");

    _assertNotNull(dir);

    dirent *dp;

    while ((dp = readdir(dir)) != NULL) {

        jclogf("path = %s[%d]", dp->d_name, dp->d_type);

    }
    closedir(dir);
}

