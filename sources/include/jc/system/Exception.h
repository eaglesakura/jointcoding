/*
 * JCException.h
 *
 *  Created on: 2012/06/13
 *      Author: Takeshi
 */

#ifndef JCEXCEPTION_H_
#define JCEXCEPTION_H_

#include    "jointcoding.h"

namespace jc {

/**
 * 例外を書き込む
 */
class Exception {

protected:
    /**
     * 現在発生中の例外型
     */
    s32 type;

    /**
     * クラス名
     */
    String className;

    /**
     * メッセージ
     */
    String message;

    /**
     * 発生箇所のファイル
     */
    String file;

    /**
     * 発生箇所のファイル
     */
    s32 line;

    /**
     * 適当な初期化を行う
     */
    Exception(const s32 type, const charactor* file, const s32 line, const charactor* message) {
        this->file = file;
        this->line = line;
        this->message = message;
        this->type = type;
    }
    virtual ~Exception() {

    }

public:
    /**
     * 例外タイプを取得する
     */
    const s32 getType() const {
        return type;
    }

    /**
     * メッセージを取得する
     */
    const String& getMessage() const {
        return message;
    }

    /**
     * 発生箇所ファイル名を取得する
     */
    const String& getFile() const {
        return file;
    }

    /**
     * 発生箇所の行数を取得する
     */
    const s32 getLine() const {
        return line;
    }

    /**
     * クラス名を取得する
     */
    const String& getClassName() const {
        return className;
    }
};

/**
 * 例外を出力する
 */
inline
void __printException(const charactor* __file, const s32 __line, const Exception *e, const charactor *msg) {
    if (!e) {
        return;
    }
    __logDebugF(LogType_Alert, __file, "L %d | %s from %s L %d has message(%s) :: log message(%s)", __line, e->getClassName().c_str(), e->getFile().c_str(), e->getLine(), e->getMessage().c_str(), msg);
}

#define jclogem(e, msg) { ::jc::__printException( ::jc::__getFileName(__FILE__), __LINE__, &e, msg); }

#define jcloge(e) { ::jc::__printException( ::jc::__getFileName(__FILE__), __LINE__, &e, "no_msg"); }

#define EXCEPTION_CLASS( name, base )    class name : public base { public: name(const s32 type, const charactor* file, const s32 line, const charactor* message) : base( type, file, line, message ){ this->className = #name; } }

/**
 * 実行時例外
 */
class RuntimeException;
EXCEPTION_CLASS(RuntimeException, Exception);

/**
 * キャスト例外
 */
class ClassCastException;
EXCEPTION_CLASS(ClassCastException, RuntimeException);

/**
 * 配列の境界エラー
 */
class ArrayIndexBoundsException;
EXCEPTION_CLASS(ArrayIndexBoundsException, RuntimeException);

/**
 * 未実装の振る舞いを行おうとした
 */
class UnsupportedOperationException;
EXCEPTION_CLASS(UnsupportedOperationException, RuntimeException);

/**
 * 仮実装の状態で、実際には利用できないメソッドを呼び出した。
 */
class StubOperationException;
EXCEPTION_CLASS(StubOperationException, RuntimeException);

/**
 * ミューテックスの取得に失敗した。
 */
class MutexLockException;
EXCEPTION_CLASS(MutexLockException, RuntimeException);

/**
 * Javaメソッド実行中の例外
 */
class JavaMethodException;
EXCEPTION_CLASS(JavaMethodException, Exception);

/**
 * IOException
 */
class IOException;
EXCEPTION_CLASS(IOException, Exception);

/**
 * ファイルが見つからなかった
 */
class FileNotFoundException;
EXCEPTION_CLASS(FileNotFoundException, IOException);

/**
 * 画像解析例外
 */
class ImageDecodeException;
EXCEPTION_CLASS(ImageDecodeException, IOException);

/**
 * Jpeg解析例外
 */
class JpegDecodeException;
EXCEPTION_CLASS(JpegDecodeException, ImageDecodeException);

enum JpegDecodeException_e {
    /**
     * Jpegのヘッダ読み込みに失敗した
     */
    JpegDecodeException_HeaderDecodeFailed,

    /**
     * ピクセル解析に失敗した
     */
    JpegDecodeException_PixelDecodeStartFailed,

    /**
     * ピクセル解析中に失敗した
     */
    JpegDecodeException_PixelDecodeingFailed,

};

/**
 * EGL初期化例外
 */
class EGLException;
EXCEPTION_CLASS(EGLException, RuntimeException);

/**
 * 例外の種類
 */
enum EGLException_e {
    /**
     * EGL初期化失敗
     */
    EGLException_InitializeFailed,
    /**
     * EGLコンフィグ取得失敗
     */
    EGLException_ConfigChooseFailed,

    /**
     * コンテキストの作成に失敗
     */
    EGLException_CreateContextFailed,

    /**
     * サーフェイス作成失敗
     */
    EGLException_SurfaceCreateFailed,

    /**
     * コンテキストのアタッチに失敗
     */
    EGLException_ContextAttachFailed,

    /**
     * コンテキストのデタッチに失敗
     */
    EGLException_ContextDetachFailed,
};

/**
 * Fbx例外
 */
class FbxException;
EXCEPTION_CLASS(FbxException, Exception);

/**
 * Fbx例外の種類
 */
enum FbxException_e {
    /**
     * シーン読み込み失敗
     */
    FbxException_SceneImportFailed,

    /**
     * ファイル初期化に失敗した
     */
    FbxException_FileInitilizeFailed,

    /**
     * Rootノードが見つからなかった
     */
    FbxException_RootNodeNotFound,

    /**
     * 既にノードが親を持っている
     */
    FbxException_NodeHasParent,

    /**
     * FbxFileとして認識出来なかった
     */
    FbxException_NoFbxFile,
};

/**
 * 新しい例外を発生させる
 */
#define     create_exception_t(Exception_class, type)      Exception_class(type, __getFileName(__FILE__), __LINE__, #type)

/**
 * 新しい例外を発生させる
 */
#define     create_exception_mt(Exception_class, type, msg)      Exception_class(type, __getFileName(__FILE__), __LINE__, msg)

/**
 * 新しい例外を発生させる
 */
#define     create_exception(Exception_class, msg)      Exception_class(0, __getFileName(__FILE__), __LINE__, msg)

#undef EXCEPTION_CLASS
}

#endif /* JCEXCEPTION_H_ */
