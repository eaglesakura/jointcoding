/*
 * FileSystem.cpp
 *
 *  Created on: 2012/07/11
 */
#include    "ndkFileSystem.h"
#include    "ndkNativeIOUtil.h"
#include    "ndkJavaJointInputStream.h"
#include    "jcException.h"

namespace ndk {

NDKFileSystem::NDKFileSystem(jc_sp< NativeContext > context) {
    this->context = context;
}

NDKFileSystem::~NDKFileSystem() {
}

/**
 * アセットからファイルを開く
 */
MInputStream NDKFileSystem::openAsset(const String fileName) {
    CALL_JNIENV();

    jobject is = NULL;
    jstring jFileName = env->NewStringUTF(fileName.c_str());
    {
        jobject local_context = context->getAppContext();
        is = NativeIOUtil::openFromAssets(jFileName, local_context);
        env->DeleteLocalRef(local_context);
    }
    env->DeleteLocalRef(jFileName);

    context->throwHasException();

    jclogf("inputstream = %x", is);

    // 正常に処理が終了したかをチェックする
    if( is ) {
        return MInputStream( new JavaJointInputStream( is ) );
    }
    else {
        throw create_exception(FileNotFoundException, "open assets failed");
    }
}

/**
 * SDカードからファイルを開く
 */
MInputStream NDKFileSystem::openExternalStrageFile(const String fileName) {
    return MInputStream();
}

}
