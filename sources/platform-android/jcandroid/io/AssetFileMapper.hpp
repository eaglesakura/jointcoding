/*
 * AssetFileMapper.hpp
 *
 *  Created on: 2013/12/14
 */
#ifndef ASSETFILEMAPPER_HPP_
#define ASSETFILEMAPPER_HPP_

#include    "jointcoding.h"
#include    "jc/io/IFileMapper.hpp"
#include    "jointcoding-android.h"

#include    <android/asset_manager_jni.h>

namespace ndk {

class AssetFileMapper: public Object, public IFileMapper {
    AAsset *asset;

    /**
     * ファイルバッファ
     */
    u8* buffer;

    /**
     * アセットサイズ
     */
    u32 _length;

    Uri uri;

    /**
     * ファイルを開く
     */
    jcboolean open(AAssetManager* pAssetManager, const Uri &uri, const FileOpenHint *hint) {
        this->uri = uri;

        asset = AAssetManager_open(pAssetManager, uri.getFileName().c_str(), AASSET_MODE_BUFFER);
        if (!asset) {
            jclogf("asset open failed :: %s", uri.getUri().c_str());
            return jcfalse;
        }

        // バッファ取得と長さチェック
        _length = AAsset_getLength(asset);
        buffer = (u8*)AAsset_getBuffer(asset);
        if (!buffer || !_length) {
            return jcfalse;
        }
        return jcfalse;
    }
public:
    AssetFileMapper() {
        asset = NULL;
        buffer = NULL;
        _length = 0;
    }
    virtual ~AssetFileMapper() {
        if (asset) {
            AAsset_close(asset);
        }
    }

    /**
     * 長さを取得する
     */
    virtual s32 length() {
        return _length;
    }

    /**
     * 頭のポインタを取得する
     */
    virtual u8* getHead() {
        return buffer;
    }

    /**
     * 元ファイルのUriを取得する
     * 不明な場合はfalseを返す。
     */
    virtual jcboolean getUri(Uri *result) {
        *result = uri;
        return jctrue;
    }
};

}

#endif /* ASSETFILEMAPPER_HPP_ */
