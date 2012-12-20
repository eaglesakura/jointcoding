# ビルド設定は全てデフォルト確定で、それ以外の使い方をする場合は自分で適宜変更する。
# project/jni/Android.mk に配置する。
#################################################################################
## setup
LOCAL_PATH := $(call my-dir)
include $(JOINTCODING_HOME)/bin/android-make/types.mk

#################################################################################
## start build
include $(CLEAR_VARS)
############################       build sources       ###########################
RELATIVITY_JC_PATH := $(shell ${JOINTCODING_HOME}/bin/abspath2rel.sh ${JOINTCODING_HOME})
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/include
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/gen
LOCAL_C_INCLUDES += $(shell find '.' -type d)
LOCAL_SRC_FILES += $(shell find '.' -name '*.c')
LOCAL_SRC_FILES += $(shell find '.' -name '*.cpp')
############################    build sources finish   ###########################
# 設定は共通のmakefileに任せる
# マクロを追加したい場合は適宜行う
LOCAL_MODULE    := $(JOINTCODING_TESTSUPPORT_MODULE)
LOCAL_LDLIBS +=  $(RELATIVITY_JC_PATH)/android/android-lib/libs/$(TARGET_ARCH_ABI)/lib$(JOINTCODING_LIB_MODULE).so
include $(JOINTCODING_HOME)/bin/android-make/jointcoding-lib-Android.mk
#################################################################################