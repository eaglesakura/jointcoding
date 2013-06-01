/*
 * jointcoding-fbx.h
 *
 *  Created on: 2012/12/14
 */

#ifndef JOINTCODING_FBX_H_
#define JOINTCODING_FBX_H_

#include    "jointcoding.h"

#include    "fbxsdk.h"
#include    "jcfbx/mem/SmartFBXObject.h"
#include    "jcfbx/FbxImportManager.h"

namespace jc {
namespace fbx {

enum Config_e {
    /**
     * 出力する1メッシュフラグメントでのボーン数
     */
    Config_MaxBones = 32,
}
;

}
}

#endif /* JOINTCODING_FBX_H_ */
