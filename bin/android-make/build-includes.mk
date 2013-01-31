############################ shared headers ###########################
# jointcoding
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/core
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/core-gl
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/gen
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/gen/protocol
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/platform-android
# Android cpufeature
# LOCAL_C_INCLUDES += $(dir $(call my-dir))../sources/cpufeatures