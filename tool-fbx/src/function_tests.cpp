/*
 * function_tests.cpp
 *
 *  Created on: 2012/12/14
 */

#include    "jointcoding-fbx.h"
#include    "jc/system/Exception.h"
#include    "jcfbx/FbxImportManager.h"
#include    "jcfbx/output/FbxExportManager.h"
#include    "jcfbx/node/Node.h"

using namespace jc;
using namespace jc::fbx;

void testFunction() {

    MNode root;

    // インポート
    {
        jc_sp<FbxImportManager> manager = FbxImportManager::createInstance("sample.fbx");

        try {
            manager->importFromSceneName("");
        } catch (Exception &e) {
            jcloge(e);
            throw;
        }

        root = manager->getConvertedRootNode();
    }

    // エクスポート
    jclogf("start export(%x)", root.get());
    {
        jc_sp<FbxExportManager> manager(new FbxExportManager());
        root->serialize(manager.get());
    }

    jclog("convert finished");
}

