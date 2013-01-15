# ビルド設定は全てデフォルト確定で、それ以外の使い方をする場合は自分で適宜変更する。
# project/jni/Android.mk に配置する。
#################################################################################
## setup
LOCAL_PATH := $(call my-dir)
include $(JOINTCODING_HOME)/bin/android-make/types.mk
#################################################################################
## start build
include $(CLEAR_VARS)

############################       build core sources       ###########################
# gen sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/gen/ -name '*.cpp')

# core sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/core/ -name '*.c')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/core/ -name '*.cpp')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/core-gl/ -name '*.cpp')

# android sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/android/ -name '*.c')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/android/ -name '*.cpp')

############################       build  app sources       ###########################
LOCAL_C_INCLUDES += $(shell find '.' -type d)
LOCAL_SRC_FILES += $(shell find '.' -name '*.c')
LOCAL_SRC_FILES += $(shell find '.' -name '*.cpp')
############################         build sources finish   ###########################
# 設定は共通のmakefileに任せる
# マクロを追加したい場合は適宜行う
# LOCAL_MODULE    := $(JOINTCODING_APP_MODULE)
LOCAL_MODULE	:= $(JOINTCODING_FULLAPP_MODULE)

include $(JOINTCODING_HOME)/bin/android-make/jointcoding-full-app-Android.mk
#################################################################################
