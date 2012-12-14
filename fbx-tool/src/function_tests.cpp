/*
 * function_tests.cpp
 *
 *  Created on: 2012/12/14
 */

#include    "jointcoding-fbx.h"
#include    "jcfbx/mem/SmartFBXObject.h"
#include    "jc/io/FileInputStream.h"

using namespace jc;

void testFunction() {

    jc::SmartFBXObject<KFbxSdkManager> manager(KFbxSdkManager::Create());
    jc::SmartFBXObject<KFbxImporter> importer(KFbxImporter::Create(manager.get(), ""));

    try {
        MInputStream is(new FileInputStream("sample.fbx"));
        jclogf("stream open ok(%x)", is.get());
    }
    catch( Exception &e ) {
        jclog("stream open ng");
    }

    importer.reset();
    manager.reset();
}

