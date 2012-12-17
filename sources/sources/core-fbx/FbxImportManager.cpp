/*
 * FbxImportManager.cpp
 *
 *  Created on: 2012/12/14
 */

#include    "jcfbx/FbxImportManager.h"
#include    "jcfbx/Node.h"

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
    scene.reset(KFbxScene::Create(sdkManager.get(), name));
    if (!scene.get()) {
        jclogf("scene create ng(%s)", name);
        throw create_exception_t(FbxException, FbxException_SceneImportFailed);
    }

    // importを行う
    if (!importer->Import(scene.get())) {
        throw create_exception_t(FbxException, FbxException_SceneImportFailed);
    } else {
        // importerは廃棄していい
        importer.reset();
    }

    jclogf("scene import ok(%s)", name);
    KFbxNode *rootNode = scene->GetRootNode();

    if (!rootNode) {
        throw create_exception_t(FbxException, FbxException_RootNodeNotFound);
    }
    jclogf("node import ok(%s)", name);
    // ノードの構築
    convertedNode = Node::createInstance(rootNode, MNode(), this);
}

/**
 * FBXインポートクラスを作成する
 * 作成に失敗した場合は例外を投げる
 */ //
jc_sp<FbxImportManager> FbxImportManager::createInstance( const charactor* fbxFileName ) {
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

    if( !importer->IsFBX()) {
        throw create_exception_t(FbxException, FbxException_NoFbxFile);
    }

    // 条件が揃ったから生成して返す
    result.reset(new FbxImportManager(manager, importer));
    return result;
}

}
}
