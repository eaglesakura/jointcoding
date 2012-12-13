/*
 * ndkViewUtil.cpp
 *
 *  Created on: 2012/10/02
 */

#include    "ndkViewUtil.h"
#include    "ndkGLNativeSurfaceViewContext.h"
#include    "ndkGLNativeTextureViewContext.h"
#include    "ndkNativeOnTouchListener.h"
#include    "jcViewOnTouchListener.h"
#include    "jcViewOnClickListener.h"
#include    "ndkSystemMemory.h"
#include    "NdkNativeOnClickListener.h"

namespace ndk {

/**
 * GLNativeSurfaceViewからGLレンダリング用のDeviceを取得する。
 * Deviceは自動的に復帰が行われる。
 */
jc::gl::MDevice ViewUtil::getDeviceFromSurfaceView(jobject GLNativeSurfaceView_instance) {
    return GLNativeSurfaceViewContext::getNativeContext(GLNativeSurfaceView_instance)->getDevice();
}

/**
 * GLNativeTexureViewからGLレンダリング用のDeviceを取得する。
 * Deviceは自動的に復帰が行われる。
 */
jc::gl::MDevice ViewUtil::getDeviceFromTextureView(jobject GLNativeTextureView_instance) {
    return GLNativeTextureViewContext::getNativeContext(GLNativeTextureView_instance)->getDevice();
}

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
