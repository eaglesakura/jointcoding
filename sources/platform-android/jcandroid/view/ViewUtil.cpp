/*
 * ndkViewUtil.cpp
 *
 *  Created on: 2012/10/02
 */

#include    "jcandroid/view/ViewUtil.h"
#include    "jcandroid/view/GLNativeSurfaceViewContext.h"
#include    "jcandroid/view/GLNativeTextureViewContext.h"
#include    "android-classes/ndkNativeOnTouchListener.h"
#include    "jc/view/OnTouchListener.h"
#include    "jc/view/OnClickListener.h"
#include    "jcandroid/jni/SystemMemory.h"
#include    "android-classes/ndkNativeOnClickListener.h"

namespace ndk {

/**
 * onClickListenerをセッティングする。
 * 古いリスナは上書きされる。
 */
void ViewUtil::setOnTouchListener(jobject view, jc_sp<Object> listener ) {
    jc::ViewOnTouchListener *uspListener = dynamic_cast<jc::ViewOnTouchListener*>( listener.get() );
    if( !uspListener ) {
        throw create_exception(ClassCastException, "listener -> ViewOnTouchListener* cast failed");
    }

    {
        jobject objMonitor = NativeOnTouchListener::setOnTouchListener_unsafe( view, (jint) uspListener);
        SystemMemory::addMonitor(objMonitor, listener);
    }

}

/**
 * onClickListenerをセッティングする。
 * 古いリスナは上書きされる。
 *
 * @param listener  jc::ViewOnClickListenerを実装したオブジェクト
 */
void ViewUtil::setOnClickListener(jobject view, jc_sp<Object> listener) {
    jc::ViewOnClickListener *uspListener = dynamic_cast<jc::ViewOnClickListener*>(listener.get());
    if(!uspListener) {
        throw create_exception(ClassCastException, "listener -> ViewOnClickListener* cast failed");
    }

    {
        jobject objMonitor = NativeOnClickListener::setOnClickListener_unsafe(view, (jint)uspListener);
        SystemMemory::addMonitor(objMonitor, listener);
    }
}

}
