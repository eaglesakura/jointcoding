/*
 * SdkDeviceManagerContext.hpp
 *
 *  Created on: 2013/05/08
 */

#ifndef SDKDEVICEMANAGERCONTEXT_HPP_
#define SDKDEVICEMANAGERCONTEXT_HPP_

#include    "jointcoding-android.h"
#include    "jc/gl/gpu/Device.h"

namespace ndk {
using namespace jc;
using namespace jc::gl;

class SdkDeviceManagerContext: public Object {
    MDevice device;
public:
    SdkDeviceManagerContext(MDevice device) {
        this->device = device;

        assert(this->device);
    }

    virtual ~SdkDeviceManagerContext() {
    }

    MDevice getDevice() const {
        return device;
    }

    /**
     * デバイスを取得する
     */
    static MDevice getDeviceFromSdkDeviceManager(jobject jDeviceManager) {
        SdkDeviceManagerContext *context = joint_context(jDeviceManager, SdkDeviceManagerContext);
        if(context) {
            return context->device;
        } else {
            return MDevice();
        }
    }
};

}

#endif /* SDKDEVICEMANAGERCONTEXT_HPP_ */
