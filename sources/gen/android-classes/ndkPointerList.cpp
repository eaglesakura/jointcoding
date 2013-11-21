/**
 * export from "Joint Coding Project"
 */
#include "ndkPointerList.h"

namespace ndk {

const ::jc::charactor* PointerList::CLASS_SIGNATURE = "com/eaglesakura/jc/android/resource/jni/PointerList";

static jclass class_PointerList = NULL;

#define methods_PointerList_LENGTH 2

#if methods_PointerList_LENGTH
static jmethodID methods_PointerList[2];
#endif

static void initialize_PointerList() {
    // loaded !
    if (class_PointerList) {
        return;
    }

    CALL_JNIENV();

    // load class object
    class_PointerList = env->FindClass(PointerList::CLASS_SIGNATURE);
    class_PointerList = (jclass)::ndk::change_globalref(env, class_PointerList);
    

    // load methods
    {
        methods_PointerList[0] = ::ndk::JniWrapper::loadMethod(class_PointerList, "remove", "(I)V", false);
        methods_PointerList[1] = ::ndk::JniWrapper::loadMethod(class_PointerList, "remove", "(Ljava/lang/String;)V", false);

    }
}

PointerList::PointerList(jobject obj): ::ndk::JniWrapper(obj){
    initialize_PointerList();
}

void PointerList::remove(jint key) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_PointerList[0], key);
}

void PointerList::remove_(jobject _this, jint key) {
    CALL_JNIENV();
    initialize_PointerList();
    env->CallVoidMethod(_this, methods_PointerList[0], key);
}

void PointerList::remove(jstring key) {
    CALL_JNIENV();
    env->CallVoidMethod(this->getObject(), methods_PointerList[1], key);
}

void PointerList::remove_(jobject _this, jstring key) {
    CALL_JNIENV();
    initialize_PointerList();
    env->CallVoidMethod(_this, methods_PointerList[1], key);
}

jc_sp<PointerList> PointerList::wrap(jobject obj) {
    return jc_sp<PointerList>( mark_new PointerList(obj));
}

jc_sp<PointerList> PointerList::global(jobject obj) {
    return jc_sp<PointerList>( (PointerList*)(mark_new PointerList(obj))->addGlobalRef());
}

jclass PointerList::getClass() {
    initialize_PointerList();
    return class_PointerList;
}

}

