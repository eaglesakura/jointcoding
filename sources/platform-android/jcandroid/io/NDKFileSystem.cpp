/*
 * FileSystem.cpp
 *
 *  Created on: 2012/07/11
 */
#include    "jcandroid/io/NDKFileSystem.h"
#include    "android-classes/ndkNativeIOUtil.h"
#include    "jcandroid/io/JavaJointInputStream.h"
#include    "jc/system/Exception.h"

namespace ndk {

NDKFileSystem::NDKFileSystem(jc_sp< NativeContext > context) {
    this->context = context;
    this->assets.reset(new JObject(context->getAssetManager_unsafe()));
    this->assets->addGlobalRef();

    CALL_JNIENV();
    this->pAssetManager = AAssetManager_fromJava(env, assets->getObject());
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
        jobject local_context = context->getAppContext_unsafe();
        is = NativeIOUtil::openFromAssets_unsafe(jFileName, local_context);
        env->DeleteLocalRef(local_context);
    }
    env->DeleteLocalRef(jFileName);

    context->throwHasException();

#ifdef LOG_JAVAJOINTINPUTSTREAM
    jclogf("inputstream = %x", is);
#endif

    // 正常に処理が終了したかをチェックする
    if( is ) {
        return MInputStream( mark_new JavaJointInputStream( is ) );
    }
    else {
        throw create_exception(FileNotFoundException, "open assets failed");
    }
}

/**
 *
 */
MInputStream NDKFileSystem::openLocalStrageFile(const String fileName) {

    CALL_JNIENV();

    jobject is = NULL;
    jstring jFileName = env->NewStringUTF(fileName.c_str());
    {
        jobject local_context = context->getAppContext_unsafe();
        is = NativeIOUtil::openFromLocalStrage_unsafe(jFileName, local_context);
        env->DeleteLocalRef(local_context);
    }
    env->DeleteLocalRef(jFileName);

    context->throwHasException();

#ifdef LOG_JAVAJOINTINPUTSTREAM
    jclogf("inputstream = %x", is);
#endif

    // 正常に処理が終了したかをチェックする
    if( is ) {
        return MInputStream( mark_new JavaJointInputStream( is ) );
    }
    else {
        throw create_exception(FileNotFoundException, "open local failed");
    }
}

/**
 * SDカードからファイルを開く
 */
MInputStream NDKFileSystem::openExternalStrageFile(const String fileName) {
    return MInputStream();
}

/**
 * ファイルを読み込む
 * ファイルのマッピング方法は実装により異なる。
 */
MFileMapper NDKFileSystem::loadFile(const Uri &uri, const FileOpenHint *hint) {
    const UriSchemeType_e scheme = uri.getSchemeType();
    if (scheme == UriSchemeType_Assets) {
        // load assets
    }

    jclogf("unknown scheme :: %s", uri.getScheme().c_str());
    return MFileMapper();
}

}
