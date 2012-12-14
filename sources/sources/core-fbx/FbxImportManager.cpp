/*
 * FbxImportManager.cpp
 *
 *  Created on: 2012/12/14
 */

#include    "jcfbx/FbxImportManager.h"

namespace jc {
namespace fbx {

FbxImportManager::FbxImportManager(KFbxSdkManager* pSdkManager, KFbxImporter *pImporter) {
    this->sdkManager.reset(pSdkManager);
    this->importer.reset(pImporter);
    nextAllocNodeId = 0;
}

FbxImportManager::~FbxImportManager() {
    scene.reset();
    importer.reset();
    sdkManager.reset();
}

void FbxImportManager::importFromSceneName(const charactor *name) {
    scene.reset(KFbxScene::Create(sdkManager.get(), NULL));
    if (!scene.get()) {
        jclogf("scene create ng(%s)", name);
        throw create_exception_t(FbxException, FbxException_SceneImportFailed);
    }

    jclogf("scene create ok(%s)", name);
    KFbxNode *rootNode = scene->GetRootNode();

    if (!rootNode) {
        throw create_exception_t(FbxException, FbxException_RootNodeNotFound);
    }
    jclogf("node create ok(%s)", name);
}

/**
 *
 */jc_sp<FbxImportManager> FbxImportManager::createInstance( const charactor* fbxFileName ) {
    jc_sp<FbxImportManager> result;

    KFbxSdkManager *manager = KFbxSdkManager::Create();
    KFbxImporter *importer = KFbxImporter::Create(manager, NULL);

    assert(manager != NULL);
    assert(importer != NULL);

    if (!importer->Initialize(fbxFileName)) {
        jclogf("importer initialize ng(%s)", fbxFileName);
        throw create_exception_t(FbxException, FbxException_FileInitilizeFailed);
    }

    jclogf("importer initialize ok(%s)", fbxFileName);

    // 条件が揃ったから生成して返す
    result.reset(new FbxImportManager(manager, importer));
    return result;
}

}
}
