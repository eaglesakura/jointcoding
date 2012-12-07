/*
 * jcViewOnTouchListener.h
 *
 *  Created on: 2012/10/05
 */

#ifndef JCVIEWONTOUCHLISTENER_H_
#define JCVIEWONTOUCHLISTENER_H_

#include    "jcTouchEventProtocol.h"

namespace jc {

class ViewOnTouchListener {
public:
    virtual ~ViewOnTouchListener() {

    }

    /**
     * タッチイベントを開始した
     */
    virtual void onTouchEventBegin(const s32 eventNum) = 0;

    /**
     * タッチイベントの処理を行う。
     */
    virtual void onTouchEvent(jc_sp<TouchEventProtocol> event, const s32 index, const s32 eventNum) = 0;

    /**
     * タッチイベントが完了した
     */
    virtual void onTouchEventComplete() = 0;
};

}

#endif /* JCVIEWONTOUCHLISTENER_H_ */
