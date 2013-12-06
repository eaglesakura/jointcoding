/**
 * export from "Joint Coding Project"
 */
#ifndef __DATABANK_H__
#define __DATABANK_H__
#include "jointcoding-android.h"

namespace ndk {

class DataBank: public ::ndk::JniWrapper {
protected:
    DataBank(jobject obj);
public:
    virtual ~DataBank(void){
    }

    static const ::jc::charactor* CLASS_SIGNATURE;
    virtual jstring popString(jlong key);
    static jstring popString_(jobject _this, jlong key);
    virtual jdouble popDouble(jlong key);
    static jdouble popDouble_(jobject _this, jlong key);
    virtual jfloat popFloat(jlong key);
    static jfloat popFloat_(jobject _this, jlong key);
    virtual jlong popLong(jlong key);
    static jlong popLong_(jobject _this, jlong key);
    virtual jint popInt(jlong key);
    static jint popInt_(jobject _this, jlong key);
    virtual jobject pop_unsafe(jlong key);
    static jobject pop_unsafe_(jobject _this, jlong key);
    virtual jlong add(jobject obj);
    static jlong add_(jobject _this, jobject obj);


    static jc_sp<DataBank> wrap(jobject obj);
    static jc_sp<DataBank> global(jobject obj);

    static jclass getClass();
};

}

#endif // __DATABANK_H__
