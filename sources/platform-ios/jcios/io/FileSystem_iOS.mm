/*
 * FileSystem.cpp
 *
 *  Created on: 2012/07/11
 */
#include    "jcios/io/FileSystem_iOS.h"
#include    "jc/system/Exception.h"


namespace ndk {

iOSFileSystem::iOSFileSystem() {
}

iOSFileSystem::~iOSFileSystem() {
}

/**
 * アセットからファイルを開く
 */
MInputStream iOSFileSystem::openAsset(const String fileName) {
    // not impl
    assert(false);
}

/**
 *
 */
MInputStream iOSFileSystem::openLocalStrageFile(const String fileName) {
    // not impl
    assert(false);
}

/**
 * SDカードからファイルを開く
 */
MInputStream iOSFileSystem::openExternalStrageFile(const String fileName) {
    return MInputStream();
}

}
