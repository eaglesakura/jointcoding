/**
 * export from "Joint Coding Project"
 */
#ifndef __CAMERAWRAPPER_H__
#define __CAMERAWRAPPER_H__
#include "jointcoding-android.h"

namespace ndk {

class CameraWrapper: public ::ndk::JniWrapper {
protected:
    CameraWrapper(jobject obj);
public:
    virtual ~CameraWrapper(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jboolean isAspectFrip();
    static jboolean isAspectFrip_(jobject _this);
    virtual jint getPreviewHeight();
    static jint getPreviewHeight_(jobject _this);
    virtual jboolean requestOrientation(jint CameraDeviceProtocol_ORIENTATION);
    static jboolean requestOrientation_(jobject _this, jint CameraDeviceProtocol_ORIENTATION);
    virtual jboolean cancelAutofocus();
    static jboolean cancelAutofocus_(jobject _this);
    virtual jboolean renderingToTexture();
    static jboolean renderingToTexture_(jobject _this);
    virtual jboolean startAutofocus();
    static jboolean startAutofocus_(jobject _this);
    virtual jboolean startPreview(jint textureName);
    static jboolean startPreview_(jobject _this, jint textureName);
    virtual void dispose();
    static void dispose_(jobject _this);
    virtual jint getOrientationType();
    static jint getOrientationType_(jobject _this);
    virtual jint getPreviewWidth();
    static jint getPreviewWidth_(jobject _this);
    virtual jint popFocusMode();
    static jint popFocusMode_(jobject _this);
    static jobject createInstance_unsafe(jint CameraDeviceProtocol_TYPE);
    virtual void requestPreviewSize(jint width, jint height, jint minWidth, jint minHeight);
    static void requestPreviewSize_(jobject _this, jint width, jint height, jint minWidth, jint minHeight);
    virtual jboolean stopPreview();
    static jboolean stopPreview_(jobject _this);
    virtual jobject getTextureMatrix_unsafe();
    static jobject getTextureMatrix_unsafe_(jobject _this);
    virtual jboolean isAutofocusProcessing();
    static jboolean isAutofocusProcessing_(jobject _this);


    static jc_sp<CameraWrapper> wrap(jobject obj);
    static jc_sp<CameraWrapper> global(jobject obj);

    static jclass getClass();
};

}

#endif // __CAMERAWRAPPER_H__
