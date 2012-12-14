/*
 * function_tests.cpp
 *
 *  Created on: 2012/12/14
 */

#include    "fbxsdk.h"
#include    "jointcoding.h"

void testFunction() {
    KFbxSdkManager *manager = KFbxSdkManager::Create();
    jclogf("manager addr(%x)", manager);
    manager->Destroy();
}

