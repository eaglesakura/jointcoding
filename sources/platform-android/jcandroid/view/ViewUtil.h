/*
 * ndkViewUtil.h
 *
 *  Created on: 2012/09/28
 */

#ifndef NDKVIEWUTIL_H_
#define NDKVIEWUTIL_H_

#include    "jointcoding-android.h"

namespace ndk {

/**
 * AndroidViewの便利メソッドを提供する
 */
class ViewUtil {
public:
    /**
     * onTouchListenerをセッティングする。
     * 古いリスナは上書きされる。
     *
     * @param listener  jc::ViewOnTouchListenerを実装したオブジェクト
     */
    static void setOnTouchListener(jobject view, jc_sp<Object> listener);

    /**
     * onClickListenerをセッティングする。
     * 古いリスナは上書きされる。
     *
     * @param listener  jc::ViewOnClickListenerを実装したオブジェクト
     */
    static void setOnClickListener(jobject view, jc_sp<Object> listener);

};

}

#endif /* NDKVIEWUTIL_H_ */
