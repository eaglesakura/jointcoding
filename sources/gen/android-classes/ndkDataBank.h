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
    virtual jdouble popDouble(jlong e);
    static jdouble popDouble_(jobject _this, jlong e);
    virtual jfloat popFloat(jlong e);
    static jfloat popFloat_(jobject _this, jlong e);
    virtual jlong popLong(jlong e);
    static jlong popLong_(jobject _this, jlong e);
    virtual jint popInt(jlong e);
    static jint popInt_(jobject _this, jlong e);
    virtual jobject pop_unsafe(jlong jlong0);
    static jobject pop_unsafe_(jobject _this, jlong jlong0);
    virtual jlong add(jobject jobject0);
    static jlong add_(jobject _this, jobject jobject0);


    static jc_sp<DataBank> wrap(jobject obj);
    static jc_sp<DataBank> global(jobject obj);

    static jclass getClass();
};

}

#endif // __DATABANK_H__
