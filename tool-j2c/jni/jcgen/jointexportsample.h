/**
 * export from "Joint Coding Project"
 */
#ifndef __JOINTEXPORTSAMPLE_H__
#define __JOINTEXPORTSAMPLE_H__
#include "jointcoding-android.h"

class JointExportSample: public JCJniWrapper {
protected:
    JointExportSample(jobject obj);
public:
    virtual ~JointExportSample(void){
    }

    static const charactor* CLASS_SIGNATURE;

    static void setHoge(jint value);
    virtual jstring setHoge(jint value, jstring str);
    virtual void nativeCall(jint arg0, jstring arg1);
    virtual jintArray doIntArray(jintArray array);
    virtual jobjectArray doHogeArray(jobjectArray clz);
    virtual jobject obj(jobject obj);
    virtual jobject equals(jobject obj);
    virtual jobjectArray doObjectArray(jobjectArray array);
    virtual jobject getHoge(jint value);
    virtual jobject doHoge(jobject clz);
    virtual void read(jbyteArray buffer, jint index, jint length);
    virtual void setHoge(void);

    JCOBJECT_GETFILENAME()

    static JointExportSample* wrap(jobject obj);
    static JointExportSample* global(jobject obj);
};
#endif // __JOINTEXPORTSAMPLE_H__
