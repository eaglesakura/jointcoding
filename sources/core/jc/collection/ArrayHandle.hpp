/*
 * ArrayHandle.hpp
 *
 *  Created on: 2013/05/17
 */

#ifndef ARRAYHANDLE_HPP_
#define ARRAYHANDLE_HPP_

#include    "jointcoding.h"
#include    "jc/collection/BitFlags.hpp"
#include    "jc/collection/SafeArray.hpp"
#include    <vector>

namespace jc {

/**
 * ハンドル情報
 * 各インスタンスで保持する
 */
struct handle_data {
    s32 index;

    handle_data() {
        index = -1;
    }

    handle_data(const s32 alloc_index) {
        index = alloc_index;
    }
};

/**
 * ハンドルのメタ情報
 * こちらは実体とは独立した配列として管理する
 */
struct handle_meta {
    /**
     * ハンドルの参照数
     * 0ならば未使用として扱う
     */
    s32 refs;

    /**
     * ハンドルに付与するマーカーフラグ
     * 最大で32個のマーカーフラグを取り付けることが出来る
     */
    BitFlags<32> extension_markers;

    handle_meta() {
        refs = 0;
    }
};

enum HandleCallback_e {
    /**
     * オブジェクトが生成された
     */
    HandleCallback_Allocated,

    /**
     * 利用を開始する
     */
    HandleCallback_Use,

    /**
     * オブジェクトの参照数が0に戻された
     */
    HandleCallback_Released,

    /**
     * for each用コールバック
     * 有効なオブジェクト
     */
    HandleCallback_Exist,

    /**
     * for each用コールバック
     * 無効なオブジェクト
     */
    HandleCallback_Unused,

    /**
     * デストラクタが起動された
     */
    HandleCallback_Destroy,
};

/**
 * ハンドルに対し何らかの処理が行われたらコールバックされる
 * @param callback コールバックの種類
 * @param pHandleTable_this 呼び出しを行ったテーブル自身へのthisポインタ
 * @param pValues 生成されたポインタ
 * @param pMetaHeader ハンドルのメタデータへのポインタ
 * @param objects 生成されたオブジェクト数
 */
typedef void (*handletable_callback)(const HandleCallback_e callback_type, void* pHandleTable_this, void* pValues, handle_meta *pMetaHeader, const u32 objects);

/**
 * ハンドルのメモリ管理の種類
 */
enum HandleType_e {
    /**
     * プリミティブ型、ポインタ型のハンドル
     * memcpyが使え、高速に動作させられる
     */
    HandleType_Primitive,

    /**
     * Class等の特殊な管理下にある型のハンドル
     * operator=が必要なため、低速なコピーメソッドで動作する
     */
    HandleType_Managed,
};

/**
 * データカプセル
 * 実体を指し示す
 */
template<typename value_type>
class handle_table {
    /**
     * 参照する実体データ配列
     */
    safe_array<value_type> values;

    /**
     * 各データの参照数管理テーブル
     */
    safe_array<handle_meta> metas;

    /**
     * 次の割り当てインデックス
     */
    s32 alloc_index;

    /**
     * 有効なオブジェクト数
     */
    s32 exist_objects;

    /**
     * ハンドルタイプ
     */
    HandleType_e type;

    /**
     * 指定位置のハンドルを取得する
     */
    inline void getHandles(const s32 header_index, handle_data *result, const s32 request_num) {
        for (int i = 0; i < request_num; ++i) {
            // 古い参照を解除する
            release(result[i]);

            // インデックスを割り当てる
            result[i] = handle_data(header_index + i);
//
//            jclogf("result_index(%d)", result[i].index);
            // 参照を追加する
            retain(result[i]);
        }
    }

    /**
     * コールバック関数ポインタ
     */
    handletable_callback callback;

    /**
     * 一度の拡張で行う最低拡張単位
     */
    s32 expansion_num;

public:
    handle_table() {
        alloc_index = exist_objects = 0;
        type = HandleType_Primitive;
        expansion_num = 1;
        callback = NULL;
    }

    ~handle_table() {
        if (callback) {
            (*callback)(HandleCallback_Destroy, (void*) this, (void*) values.ptr, metas.ptr, values.length);
        }
    }

    /**
     * 一度の領域拡張で拡張する最小単位を指定する
     * デフォルトは１個単位で拡張する
     */
    void setExpansionNum(const s32 num) {
        expansion_num = num;
    }

    /**
     * ハンドルの管理タイプを指定する
     */
    void setType(const HandleType_e type) {
        this->type = type;
    }

    /**
     * 配列の個数予約を行う
     * 事前に個数がわかっている場合はこのメソッドを呼び出すことでメモリの最確保を抑えることが出来る
     * 効率的に扱うため、呼び出すことを推奨する。
     *
     * コールバックには全オブジェクトが呼び出される
     */
    inline void reserve(const s32 _length) {
        assert(_length > values.length);

        s32 request_length = values.length;
        // 拡張数を指定する
        while (request_length < _length) {
            request_length += expansion_num;
        }

        // 古いポインタのサイズ
        const s32 old_length = values.length;
        // 各配列をリサイズする
        if (type == HandleType_Primitive) {
            values.alloc(request_length);
        } else {
            values.reserve(request_length);
        }
        // メタデータは常にmemcpyで問題ない型を利用する
        metas.alloc(request_length);

        // コールバックを呼び出す
        if (callback) {
            const s32 added = (request_length - old_length);
            (*callback)(HandleCallback_Allocated, (void*) this, (void*) (values.ptr + old_length), (metas.ptr + old_length), added);
        }
    }

    /**
     * ハンドルテーブルの格納数を取得する
     * この数までは高速にハンドル割り当てができ、それを超えると自動的に拡張を行う
     */
    inline s32 getTableNum() const {
        return values.length;
    }

    /**
     * 割り当て済みのハンドル数を取得する
     */
    inline s32 getAllocatedHandleNum() const {
        return alloc_index;
    }

    /**
     * 有効な値数を取得する
     */
    inline s32 getExistValueNum() const {
        return exist_objects;
    }

    /**
     * ハンドルが有効な場合true
     */
    inline jcboolean exist(const handle_data &handle) const {
        if (handle.index < 0) {
            return jcfalse;
        }

        if (metas[handle.index].refs == 0) {
            return jcfalse;
        }

        return jctrue;
    }

    /**
     * 参照カウントを一つ繰り上げる
     */
    inline const handle_data& retain(const handle_data &handle) {
        // 無効なハンドルならば何もしない
        if (handle.index < 0) {
            return handle;
        }
        // メタ情報を取得する
        handle_meta &meta = metas[handle.index];

        // 有効なオブジェクト数を数える
        if (!meta.refs) {
            ++exist_objects;
            // 利用を開始する
            if (callback) {
                (*callback)(HandleCallback_Use, (void*) this, (void*) (&values.ptr[handle.index]), &meta, 1);
            }
        }

        // メタテーブルを書き換える
        ++meta.refs;

        return handle;
    }

    /**
     * ハンドルを開放し、残りの有効数を返す
     */
    inline s32 release(handle_data &handle) {
        // 無効なハンドルならば何もしない
        if (!exist(handle)) {
            return 0;
        }

        // 参照数の検証
        assert((metas[handle.index].refs) > 0);

        // メタ情報を取得する
        handle_meta &meta = metas[handle.index];
        // 参照数を減らす
        --meta.refs;

        // 無効なオブジェクトとして扱う
        if (!meta.refs) {
            // 無効になったらコールバック
            if (callback) {
                (*callback)(HandleCallback_Released, (void*) this, (void*) (&values[handle.index]), &metas[handle.index], 1);
            }

            // 有効なオブジェクト数を減らす
            --exist_objects;

            // ビットフラグをリセット
            meta.extension_markers.reset();
        }

        // ハンドルのインデックスを無効化する
        handle.index = -1;

        return meta.refs;
    }

    /**
     * 値を取得する
     */
    inline value_type& get(const handle_data &handle) {
        return values[handle.index];
    }

    /**
     * 値を取得する
     */
    inline const value_type& get(const handle_data &handle) const {
        return values[handle.index];
    }

    /**
     * 指定した数のハンドルを新規に確保する
     */
    inline void allocHandles(handle_data *result, const s32 request_num) {
        // 数が足りていないならチェックする
        if ((alloc_index + request_num) > getTableNum()) {
            // 再度数を確保する
            reserve(getTableNum() + request_num);
        }

        // ハンドル位置を確保する
        getHandles(alloc_index, result, request_num);

        // 次の割り当てヘッダを指定する
        alloc_index += request_num;
    }

    /**
     * 連続した空き領域があるハンドルインデクスを検索する
     * 空き領域の連続が存在しない場合負の値を返す
     */
    inline s32 findFreeHandles(const s32 length) const {
        // 全領域からlength数を引いた数（配列がはみ出さないように）をチェック
        for (s32 i = 0; i < (getTableNum() - length); ++i) {

            jcboolean finded = jctrue;
            // 連続領域をチェック
            for (s32 k = 0; k < length; ++i) {
                // 参照数が存在する場合は次へ進む
                if (metas[i + k].refs) {
                    finded = jcfalse;
                    break;
                }
            }

            // フラグがOKの場合は現在のインデックスを返す
            if (finded) {
                return i;
            }
        }

        return -1;
    }

    /**
     * 空いているハンドルを優先して取得する
     * 空いていなかった場合、falseを返す
     */
    inline jcboolean allocFreeHandle(handle_data *result, const s32 length) {

        const s32 header_index = findFreeHandles(length);
        if (header_index >= 0) {
            // 見つかった場合はそれを取得する
            getHandles(header_index, result, length);

            // 見つかったことを返却する
            return jctrue;
        }

        // 見つからなかったことを返却する
        return jcfalse;
    }

    /**
     * 空いているハンドルを探し、見つからなかったら新規にハンドルを割り当てる
     */
    inline void allocFreeOrNewHandle(handle_data *result, const s32 length) {
        // 割り当て済みを探す
        if (allocFreeHandle(result, length)) {
            return;
        }

        // 新規割り当て
        allocHandles(result, length);
    }

    /**
     * コールバック関数を設定する
     */
    inline void setCallback(const handletable_callback call) {
        this->callback = call;
    }

    /**
     * 全オブジェクトに対して、定形のコールバックを行う
     */
    inline void executeExistOrUnused(s32 *result_unused = NULL) {
        assert(callback);

        s32 unused = 0;
        for (int i = 0; i < alloc_index; ++i) {
            // 未使用のマークをつける
            HandleCallback_e type = HandleCallback_Unused;
            if (metas[i].refs) {
                // 参照つきなら切り替える
                type = HandleCallback_Exist;
            } else {
                ++unused;
            }

            // コール
            (*callback)(type, (void*) this, (void*) (values.ptr + i), metas.ptr + i, 1);
        }

        if (result_unused) {
            *result_unused = unused;
        }
    }
};

/**
 * 実アクセスを担うハンドルクラス
 * ポインタのように
 */
template<typename value_type>
class Handle {
    /**
     * マスターデータ
     */
    jc_sp<handle_table<value_type> > master;

    /**
     * マスターデータへのポインタ
     * 高速アクセスのため、実体アクセスはこちらを利用する
     */
    handle_table<value_type> *pMaster;

    /**
     * 自身のハンドルデータ
     */
    handle_data self;

public:
    Handle() {
        pMaster = NULL;
    }

    /**
     * ハンドルコピーを行う
     */
    Handle(const Handle &cpy) {
        pMaster = NULL;
        reset(cpy.self, cpy.master);
    }

    ~Handle() {
        reset(); // 参照しているポインタを無効化する
    }

    /**
     * ハンドルが有効な場合true
     */
    jcboolean exist( ) const {
        if(!master) {
            return jcfalse;
        }

        if(master->exist(self)) {
            return jctrue;
        } else {
            return jcfalse;
        }
    }

    /**
     * 新規にハンドルを割り当てる
     */
    void alloc( ) {
        assert(pMaster);
        // 古いハンドルを廃棄する
        pMaster->release(self);
        // 新たなハンドルを取得する
        pMaster->allocFreeOrNewHandle( &self, 1);
    }

    /**
     * テーブル拡張をせずに、既に空いているハンドルを探して割り当てる
     * find alloc
     */
    jcboolean falloc( ) {
        assert(pMaster);
        // 古いハンドルを廃棄する
        pMaster->release(self);
        // 新たなハンドルを取得する
        return pMaster->allocFreeHandle( &self, 1);
    }

    /**
     * テーブルを明示的に拡張して割り当てる
     */
    void alloc_back( ) {
        assert(pMaster);
        // 古いハンドルを廃棄する
        pMaster->release(self);
        // 新たなハンドルを取得する
        pMaster->allocHandles( &self, 1);
    }

    /**
     * ハンドルをリセットする
     */
    void reset( const handle_data &handle, const jc_sp<handle_table<value_type> > master) {
        if(pMaster) {
            pMaster->release(self);
        }

        // マスターデータとハンドルを書き換える
        self = handle;
        {
            this->master = master;
            pMaster = master.get();

        }
        // ハンドルが有効ならretainする
        if(pMaster) {
            pMaster->retain(self);
        }
    }

    /**
     * ハンドルリセットを行い、無効化する
     */
    void reset(const jc_sp<handle_table<value_type> > master) {
        reset(handle_data(), master);
    }

    /**
     * ハンドルリセットを行い、無効化する
     */
    void reset() {
        reset(handle_data(), jc_sp<handle_table<value_type> >());
    }

    /**
     * 値を取得する
     */
    const value_type& get() const {
        assert(master);
        return pMaster->get(self);
    }

    /**
     * 値を取得する
     */
    value_type& get() {
        assert(master);
        return pMaster->get(self);
    }

    /**
     * 値の設定を行う
     */
    void set( const value_type &value ) {
        assert(master);
        pMaster->get(self) = value;
    }

    /**
     * オペレータアクセスを行う
     * オペレータ時は必ず参照が存在しなければならない
     */
    const value_type& operator->() const {
        assert(self.index >= 0);
        return get();
    }

    /**
     * オペレータアクセスを行う
     */
    value_type& operator->() {
        assert(self.index >= 0);
        return get();
    }

    /**
     * 要素アクセスを行う
     */
    value_type& operator*() {
        assert(self.index >= 0);
        return get();
    }

    /**
     * 参照の上書きを行う
     */
    Handle<value_type>& operator=(const Handle<value_type> cpy) {
        reset(cpy.self, cpy.master);
        return (*this);
    }
};

}

#endif /* ARRAYHANDLE_HPP_ */
