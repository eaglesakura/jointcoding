#ifndef com_eaglesakura_ios_sample_ObjectivePointer_h
#define com_eaglesakura_ios_sample_ObjectivePointer_h

#include "jointcoding-ios.h"

namespace ios {
    
    /**
     * Objc <--> C++のブリッジのため、Objective-Cのポインタをreatinを正常に保って保持するクラス
     * 何らかの目的で、Objective-Cクラスを引数指定する必要がある場合に利用する
     */
    template<typename OBJC_TYPE>
    class   ObjectivePointer {
    public:
        
        OBJC_TYPE   object;

        ObjectivePointer() {
            object = nil;
        }
        
        ObjectivePointer( OBJC_TYPE obj ) {
            object = nil;
            object = obj;
        }

        ObjectivePointer(const ObjectivePointer &cpy) {
            object = nil;
            object = cpy.object;
        }
        
        ~ObjectivePointer() {
            object = nil;
        }
        
        /**
         * 有効である場合trueを返す
         */
        operator bool() const {
            return object != nil;
        }
        
        
        /**
         * 無効である場合trueを返す
         */
        bool operator!() const {
            return object == nil;
        }
    };
    
    /**
     * id参照のラッパ
     */
    typedef ObjectivePointer<__strong id>    jc_id;
    
    /**
     * weak id参照のラッパ
     */
    typedef ObjectivePointer<__weak id> jc_weak_id;
}


#endif
