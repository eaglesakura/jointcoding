/*
 * FbxImportContext.h
 *
 *  Created on: 2012/12/14
 */

#ifndef FBXIMPORTMANAGER_H_
#define FBXIMPORTMANAGER_H_

#include    "jointcoding-fbx.h"


namespace jc {
namespace fbx {

class Node;
/**
 * FBXインポート時の管理を行う。
 * FBX1ファイルにつき、1インスタンスを利用する。
 */
class FbxImportManager {
    /**
     * 次に割り当てるノード番号。
     * Fbxファイルはインポート時に全てのノードに対して通し番号を割り当てる。
     */
    s32 nextAllocNodeId;

    /**
     * SDKの根幹となるクラス
     */
    SmartFBXObject<KFbxSdkManager> sdkManager;

    /**
     * SDK内蔵のインポーター
     */
    SmartFBXObject<KFbxImporter> importer;

    /**
     * 読み込み対象のシーン
     */
    SmartFBXObject<KFbxScene> scene;

    /**
     * 構築したルートノード
     */
    jc_sp<jc::fbx::Node> convertedNode;

    FbxImportManager(KFbxSdkManager* sdkManager, KFbxImporter *importer);
public:
    virtual ~FbxImportManager();

    /**
     * 次のノード番号を割り当てる
     */
    virtual s32 nextNodeId() {
        return nextAllocNodeId++;
    }

    /**
     * シーン名から読み込む.
     * デフォルトシーンは NULL か ""
     */
    virtual void importFromSceneName(const charactor* name);

    /**
     *
     */
    virtual KFbxSdkManager* getSdkManager() {
        return sdkManager.get();
    }

    /**
     *
     */
    virtual KFbxImporter* getImporter() {
        return importer.get();
    }

    /**
     *
     */
    static jc_sp<FbxImportManager> createInstance( const charactor* fbxFileName );
};

}
}

#endif /* FBXIMPORTCONTEXT_H_ */
