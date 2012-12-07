/*
 * OrderAccessList.h
 *
 *  Created on: 2012/08/21
 */

#ifndef ORDERACCESSLIST_H_
#define ORDERACCESSLIST_H_

#include    "jcMacro.h"
#include    "jcSmartPtr.h"
#include    "jcMutex.h"
#include    "jcException.h"

namespace jc {

template<typename T>
class Element {
public:
    /**
     * データ本体
     */
    T data;

    /**
     * 次の要素は強参照
     */
    jc_sp< Element<T> > next;

    /**
     * 前の要素は弱参照
     */
    jc_wp< Element<T> > before;

    Element(T data) {
        this->data = data;

        jclogf("alloc %x", this);
    }

    ~Element() {
        jclogf("destroy %x", this);
    }
};

/**
 * 拡張性のある順アクセスに最適化したリスト。
 */
template<typename T>
class OrderAccessList {

    typedef jc_sp< Element<T> > MElement;

    /**
     * 最初の要素
     */
    jc_sp< Element<T> > first;

    /**
     * 最後の要素
     */
    MElement last;

    /**
     * 要素数
     */
    s32 elements;

    /**
     * 排他制御ミューテックス
     */
    mutable jcmutex mutex;

    MElement newElement(T data) {
        return MElement(new Element<T>(data));
    }

    void destroyElements( ) {
        if(!first ) {
            return;
        }

        // 最後のオブジェクトのチェインは予め切っておく
        last->next.reset();
        last->before.reset();

        // チェインをたどって開放する
        {
            MElement current = first;
            while( current ) {
                MElement temp = current->next;

                // 保持しているアイテムをリセットする
                current->before.reset();
                current->next.reset();

                // 次のアイテムを見る
                current = temp;
            }
        }
        // 最初と最後の要素も開放する
        first.reset();
        last.reset();
    }

public:
    OrderAccessList() {
        elements = 0;
    }

    ~OrderAccessList() {
        destroyElements();
    }

    /**
     * 要素を全て削除する
     */
    void clear() {
        destroyElements();
        elements = 0;
    }

    /**
     * 要素を末尾に追加する
     */
    void add(const T obj) {
        add(elements, obj);
    }

    /**
     * 要素を指定インデックスになるように追加する。
     * その他のオブジェクトはひとつ後ろに下がる。
     */
    void add(const s32 index, const T obj) {
        MutexLock lock(mutex);
        if( index > elements ) {
            throw create_exception(ArrayIndexBoundsException, "index > elements");
        }

        if (!first) {
            // 最初の要素の場合
            first = newElement(obj);
            last = first;
        } else {

            // elem0 -> elem1 -> elem2の順になるようにチェインを再構築する
            // 先頭要素
            MElement elem0;
            // 次の要素を足す
            MElement elem1 = newElement(obj);
            // 後部要素
            MElement elem2;

            if( index == elements ) {
                // 末尾に追加する場合はlastを直接使える
                elem0 = last;

            } else if( index == 0 ) {
                elem2 = first;
            } else {
                elem0 = first;
                // 指定インデックスまで引き伸ばす
                for( s32 i = 0; i < index; ++i ) {
                    elem0 = elem0->next;
                }
                elem2 = elem0->next;
            }
            // 要素を交換する
            if(elem2) {
                elem1->next = elem2;
                elem2->before = elem1;
            }
            if( elem0 ) {
                elem0->next = elem1;
                elem1->before = elem0;
            }

            if( index == 0 ) {
                // 先頭要素の場合は切り替える
                first = elem1;
            } else if( index == elements ) {
                // 終端要素の場合も切り替える
                last = elem1;
            }
        }

        ++elements;
    }

    /**
     * 要素数を取得する
     */
    s32 size() const {
        MutexLock lock(mutex);
        return this->elements;
    }

    template<typename _T>
    class _Iterator {
        MElement currentElement;

        MElement nextElement;

        /**
         * 親クラス
         */
        OrderAccessList<T> *parent;

    public:
        _Iterator( OrderAccessList<T> *parent, MElement first ) {
            nextElement = first;
            this->parent = parent;
        }

        /**
         * 次の要素を持っている場合true
         */
        jcboolean hasNext() {
            return nextElement != NULL;
        }

        /**
         * 次の要素を取り出す
         * ppResult != NULLの場合、dataのポインタが格納される。
         * dataのポインタはremove()が行われるまで存在を保証する。
         */
        _T next( _T **ppResult = NULL ) {
            MutexLock lock( parent->mutex );

            currentElement = nextElement;
            nextElement = nextElement->next;
            if(ppResult) {
                (*ppResult) = &currentElement->data;
            }
            return currentElement->data;
        }

        /**
         * 現在の要素を削除する
         */
        void remove() {
            MutexLock lock( parent->mutex );

            MElement elem0 = currentElement->before.lock();
            MElement elem1 = currentElement;
            MElement elem2 = currentElement->next;

            elem1->before.reset();
            elem1->next.reset();

            if( elem0 ) {
                elem0->next = elem2;
            }
            if( elem2 ) {
                elem2->before = elem0;
            }

            // 次に実行する要素を切り替える
            {
                currentElement.reset();
                nextElement = elem2;
            }

            // 親の要素を減らす
            --parent->elements;

            // 要素が無くなったら親のリセット
            if( !parent->elements ) {
                parent->first.reset();
                parent->last.reset();
            }
        }
    };

    typedef jc_sp<_Iterator<T> > Iterator;

    /**
     * イテレータオブエジェクトを返す
     */
    Iterator iterator() {
        return Iterator(new _Iterator<T>( this, first ));
    }

    void debug() {

        Iterator itr = iterator();
        while( itr->hasNext() ) {
            T data = itr->next();
            jclogf("current :: %d", data);
        }

        /*
         MElement current = first;

         while(current) {
         jclogf("current = %x :: %d", current.get(), current->data);
         current = current->next;
         };
         */
    }

};

}

#endif /* ORDERACCESSLIST_H_ */
