/*
 * SpriteBatchList.h
 *
 *  Created on: 2013/12/03
 */
#ifndef SPRITEBATCHLIST_H_
#define SPRITEBATCHLIST_H_

#include    "jc/framework/assets/image/SpriteBatch.h"

namespace jc {

namespace gl {

/**
 * スプライトバッチをリスト化して、統一したUIでレンダリングを継続できるようにする
 */
class SpriteBatchList: public Object {
public:
    typedef typename std::list<MSpriteBatch, StlAllocator<MSpriteBatch> > batch_container;

private:
    /**
     * アップロード対象のバッチ
     */
    MSpriteBatch uploadList;

    /**
     * 操作対象のスプライトバッチ
     */
    MSpriteBatch current;

    /**
     * 複数スレッドからGPU関連処理で利用するミューテックス
     */
    jcmutex gpumutex;

protected:
    /**
     * バッチを生成させる
     */
    virtual MSpriteBatch newBatch(MDevice device, const s32 userData) = 0;
public:
    SpriteBatchList();

    virtual ~SpriteBatchList();

    /**
     * テクスチャ描画を開始する
     */
    virtual void beginBatch(MDevice device, MTextureImage texture, const s32 userData);

    /**
     * ポリゴン描画を開始する
     */
    virtual void beginBatch(MDevice device, const s32 userData);

    /**
     * 操作対象のバッチクラスを取得する
     */
    virtual MSpriteBatch getBatch() const {
        return current;
    }

    /**
     * GPUへのアップロードを行う
     */
    virtual void commit(MDevice device);

    /**
     * レンダリングを行う
     */
    virtual void rendering(MDevice device);
};

}

}

#endif /* SPRITEBATCHLIST_H_ */
