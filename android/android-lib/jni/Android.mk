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
# joint codingへの相対パス
RELATIVITY_JC_PATH := $(shell ${JOINTCODING_HOME}/bin/abspath2rel.sh ${JOINTCODING_HOME})

# gen sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/gen/ -name '*.cpp')

# core sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/core/ -name '*.c')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/core/ -name '*.cpp')

# android sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/android/ -name '*.c')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/sources/android/ -name '*.cpp')
############################    build sources finish   ###########################
# 設定は共通のmakefileに任せる
# マクロを追加したい場合は適宜行う
LOCAL_MODULE    := $(JOINTCODING_LIB_MODULE)
include $(JOINTCODING_HOME)/bin/android-make/jointcoding-lib-Android.mk
#################################################################################