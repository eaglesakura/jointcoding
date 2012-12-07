/*
 * UriTest.cpp
 *
 *  Created on: 2012/07/22
 *
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "boost/smart_ptr/shared_ptr.hpp"
#include    "jcSmartPtr.h"
#include    "jcUri.h"
#include    "jcUriProtocol.h"

using namespace jc;

extern "C" {

TESTFUNCTION void Uri_checmeTest() {
    Uri uri = Uri::fromAssets("test.txt");

    jclogf("scheme = %s", uri.getScheme().c_str());
    _assertTrue( strcmp( uri.getScheme().c_str(), UriProtocol::SCHEME_APPLI_ASSETS ) == 0);
}

TESTFUNCTION void Uri_filenameTest() {
    String  fileName = "test.txt";
    Uri uri = Uri::fromAssets(fileName);

    jclogf("path = %s", uri.getPath().c_str());
    _assertTrue( strcmp( uri.getPath().c_str(), fileName.c_str() ) == 0);
}

}

