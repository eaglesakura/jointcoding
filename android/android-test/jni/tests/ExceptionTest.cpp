/*
 * ExceptionTest.cpp
 *
 *  Created on: 2012/07/28
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "jcSmartPtr.h"
#include    "jcUri.h"
#include    "jcJpegImageDecoder.h"
#include    "boost/timer.hpp"

using namespace jc;

extern "C" {

TESTFUNCTION void Exception_throwTest() {

#ifdef  __ARM_V7__
    jclog("define __ARM_V7__");
#else
    jclog("not define __ARM_V7__");
#endif

    try {

        jclog("throw ioe");
        throw create_exception(IOException, "throw ioe");
    } catch (const IOException &e) {
        jclog("IOException");
        jcloge(e);
    } catch (const Exception &e) {
        jclog("Exception");
        jcloge(e);
    }

    try {

        jclog("throw ioe");
        throw create_exception_t(EGLException, EGLException_ContextAttachFailed);
    } catch (const IOException &e) {
        jclog("IOException");
        jcloge(e);
    } catch (const Exception &e) {
        jclog("Exception");
        jcloge(e);
    }
}

}

