/*
 * function_tests.cpp
 *
 *  Created on: 2012/12/14
 */

#include    "jointcoding-fbx.h"
#include    "jcfbx/FbxImportManager.h"
#include    "jc/system/Exception.h"

using namespace jc;
using namespace jc::fbx;

void testFunction() {

    jc_sp<FbxImportManager> manager = FbxImportManager::createInstance("sample.fbx");

    try {
        manager->importFromSceneName("");
    } catch( Exception &e ) {
        jcloge(e);
        throw;
    }

    jclog("finished");
}

