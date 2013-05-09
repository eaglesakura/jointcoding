/*
 * esBenchmarkApplication.h
 *
 *  Created on: 2013/05/06
 */

#ifndef ESBENCHMARKAPPLICATION_H_
#define ESBENCHMARKAPPLICATION_H_

#include    "benchmark.hpp"

namespace es {

class BenchmarkApplication: public JointApplicationBase {
    MTextureImage texture;

public:
    BenchmarkApplication();
    virtual ~BenchmarkApplication();

public:
    /* アプリライフサイクル */

    /**
     * アプリ初期化を行わせる
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppInitialize();

    virtual void loadTexture(MDevice subDevice);

    /**
     * アプリのサーフェイスリサイズを行わせる
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppSurfaceResized(const s32 width, const s32 height);

    /**
     * メインループを行わせる
     * メソッド呼び出し時点でデバイスロック済み
     */
    virtual void onAppMainUpdate();

    /**
     * メインレンダリングを行わせる。
     * メソッド呼び出し時点でデバイスロック済み。
     * フロントバッファ転送もメソッド内で行わせる。
     */
    virtual void onAppMainRendering();

    /**
     * アプリ休止処理
     * 音声の休止等の処理を行う
     */
    virtual void onAppPause();

    /**
     * アプリの再開処理
     * 音声の再開等の処理を行う
     */
    virtual void onAppResume();

    /**
     * アプリのメモリ解放を行う
     * メソッド呼び出し時点でデバイスロック済み。
     */
    virtual void onAppDestroy();

};

}

#endif /* ESBENCHMARKAPPLICATION_H_ */
