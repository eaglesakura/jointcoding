/*
 * MemoryCheck.cpp
 *
 *  Created on: 2013/11/14
 */

#include    "jointcoding.h"
#include    <list>

/**
 * 実装部はboostに一任する
 */
#include    "jc/thread/prv_Thread.h"

namespace jc {

namespace {

static native_mutex g_memcheck_lock;

/**
 *
 */
class MemoryMark {
    /**
     * ファイル名
     */
    char* fileName;

    /**
     * ファイル行
     */
    int fileLine;

    /**
     * 該当ファイルで生成されたポインタ
     */
    std::list<void*> pointers;
public:

    MemoryMark(char* fileName, const int fileLine) {
        this->fileName = fileName;
        this->fileLine = fileLine;
    }

    ~MemoryMark() {

    }

    /**
     * マーカーを持っている場合はtrue
     */
    jcboolean isEmpty() {
        return pointers.empty();
    }

    /**
     * メモリ情報をダンプする
     */
    void dump() {
        if (isEmpty()) {
            jclogf("%s L%d [no markers]", fileName, fileLine);
        } else {
            jclogf("%s L%d [has %d markers]", fileName, fileLine, pointers.size());
        }

//        std::list<void*>::const_iterator itr = pointers.begin(), end = pointers.end();
//        while (itr != end) {
//            ++itr;
//        }
    }

    /**
     * 指定したポインタを持っている場合はtrue
     */
    jcboolean hasPointer(const void* pointer) {

        std::list<void*>::const_iterator itr = pointers.begin(), end = pointers.end();

        while (itr != end) {

            // 一致するポインタを見つけた
            if ((*itr) == pointer) {
                return jctrue;
            }

            ++itr;
        }

        return jcfalse;
    }

    /**
     * マークを付与する
     */
    void mark(const void* p) {
        if (!hasPointer(p)) {
            pointers.push_back((void*) p);
        }
    }

    /**
     * マークを外す
     */
    jcboolean unmark(const void* p) {
        std::list<void*>::iterator itr = pointers.begin(), end = pointers.end();

        while (itr != end) {
            if ((*itr) == p) {
                pointers.erase(itr);
                return jctrue;
            }

            ++itr;
        }
        return jcfalse;
    }

    /**
     *
     */
    jcboolean isMarker(const char* file, const int line) {
        // __FILE__で比較するため、この方法で問題ない
        return (this->fileName == file) && (this->fileLine == line);
    }
};

/**
 * メモリマーク
 */
static std::list<MemoryMark*> marks;

/**
 * 指定したファイル/行に一致するマーカーを探す
 */
static MemoryMark* findMark(const char* file, const int line) {
    {
        std::list<MemoryMark*>::const_iterator itr = marks.begin(), end = marks.end();
        while (itr != end) {
            if ((*itr)->isMarker(file, line)) {
                return (*itr);
            }
            ++itr;
        }
    }

    // 新規にマークを作成する
    MemoryMark *mark = new MemoryMark((char*) file, line);
    marks.push_back(mark);
    return mark;
}
}

/**
 * マークを付ける
 */
void MemoryCheck::mark(void* memory, const char* file, const int line) {
    native_lock lock(g_memcheck_lock);

    // 排他制御を行う
    {
        MemoryMark *mark = findMark(__getFileName(file), line);
        // マークを与える
        mark->mark(memory);
    }
}

/**
 * リークマークを外す
 */
void MemoryCheck::unmark(void* memory, const char* file, const int line) {
    native_lock lock(g_memcheck_lock);

    // 排他制御を行う
    {
        std::list<MemoryMark*>::iterator itr = marks.begin(), end = marks.end();
        while (itr != end) {
            // マーカーを排除
            (*itr)->unmark(memory);
            ++itr;
        }
    }

}

/**
 * 個数ダンプを行う
 */
void MemoryCheck::dump() {
    native_lock lock(g_memcheck_lock);

    jclog("----------------------")
    {
        std::list<MemoryMark*>::const_iterator itr = marks.begin(), end = marks.end();
        while (itr != end) {
            MemoryMark *mark = (*itr);
            // ログ出力を行う
            {
                mark->dump();
            }
            ++itr;
        }
    }
}

}

