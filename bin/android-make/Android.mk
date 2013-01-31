# ビルド設定は全てデフォルト確定で、それ以外の使い方をする場合は自分で適宜変更する。
# project/jni/Android.mk に配置する。
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

############################ shared headers ###########################
# app includes
LOCAL_C_INCLUDES +=  $(shell find './' -type d)
# app sources
LOCAL_SRC_FILES += $(shell find './' -name '*.cpp')
LOCAL_SRC_FILES += $(shell find './' -name '*.c')
############################ shared headers ###########################

# 設定は共通のmakefileに任せる
include $(JOINTCODING_HOME)/bin/android-make/jointcoding-Android.mk