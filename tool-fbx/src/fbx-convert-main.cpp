/*
 * fbx-convert-main.cpp
 *
 *  Created on: 2013/05/19
 */

#include    "jointcoding-fbx.h"
#include    "jc/system/Exception.h"
#include    "jcfbx/FbxImportManager.h"
#include    "jcfbx/output/FbxExportManager.h"
#include    "jcfbx/node/Node.h"

using namespace jc;
using namespace jc::fbx;

/**
 * FBXファイルをバイナリに変換する
 */
void fbx2binary(const String &fbx3ds_file, const String &dst_file) {

    // まずはバイナリコンバート
    {
        MNode root;
        // インポート
        {
            jc_sp<FbxImportManager> manager = FbxImportManager::createInstance(fbx3ds_file.c_str());
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

            manager->serialize(root);
        }
        jclog("convert finished");
    }

    // 変換を行う
    {
        extern void makeFileArchive(const String &dst);
        makeFileArchive(dst_file);
    }
}

