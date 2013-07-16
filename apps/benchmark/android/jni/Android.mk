# ビルド設定は全てデフォルト確定で、それ以外の使い方をする場合は自分で適宜変更する。
# project/jni/Android.mk に配置する。
#################################################################################
LOCAL_PATH := $(call my-dir)
#################################################################################
## start build
include $(CLEAR_VARS)
## setup
RELATIVITY_JC_PATH := $(shell ${JOINTCODING_HOME}/bin/abspath2rel.sh ${JOINTCODING_HOME})
############################       app flags       ###########################
## Joint Coding
# LOCAL_CPPFLAGS += -DOUTPUT_LOG
# GLAssert無効（速度チェック用）
LOCAL_CPPFLAGS += -DNO_GL_ASSERT
LOCAL_CPPFLAGS += -DSUPPORT_TEGRA_PERFHUDES
# LOCAL_CPPFLAGS += -DEGL_TRIPLEBUFFER_MODE
# LOCAL_CPPFLAGS += -DRENDERING_VIEWAREA



############################       build core sources       ###########################
# gen sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/gen/ -name '*.cpp')

# core sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/core/ -name '*.c')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/core/ -name '*.cpp')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/core-gl/ -name '*.c')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/core-gl/ -name '*.cpp')

# android sources
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/platform-android/ -name '*.c')
LOCAL_SRC_FILES += $(shell find $(RELATIVITY_JC_PATH)/sources/platform-android/ -name '*.cpp')


############################       build  app sources       ###########################
LOCAL_C_INCLUDES += $(shell find '.' -type d)
LOCAL_SRC_FILES += $(shell find '.' -name '*.c')
LOCAL_SRC_FILES += $(shell find '.' -name '*.cpp')
############################         build sources finish   ###########################
# 設定は共通のmakefileに任せる
# マクロを追加したい場合は適宜行う
# LOCAL_MODULE    := $(JOINTCODING_APP_MODULE)
LOCAL_MODULE	:= jcapp
############################ shared headers ###########################
# jointcoding
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/core
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/core-gl
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/gen
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/gen/protocol
LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/sources/platform-android

# support OpenGL ES 2.0
# LOCAL_C_INCLUDES += $(RELATIVITY_JC_PATH)/glm

############################ make flags     ###########################
# 例外を有効
LOCAL_CPPFLAGS   += -fexceptions
# スレッド有効
LOCAL_CPPFLAGS   += -pthread
# RTTIを有効
LOCAL_CPPFLAGS   += -frtti
# 警告無効
LOCAL_CFLAGS     += -Wno-psabi
# マクロ定義
LOCAL_CPPFLAGS   += -D__ARM_V7__ 

# NEON
# LOCAL_CPPFLAGS += -mfpu=neon
# LOCAL_CPPFLAGS += -mfloat-abi=softfp
# LOCAL_CFLAGS += -mfpu=neon
# LOCAL_CFLAGS += -mfloat-abi=softfp

############################ import libs   ###########################
LOCAL_LDLIBS 			+= -lGLESv2
LOCAL_LDLIBS 			+= -lEGL
LOCAL_LDLIBS 			+= -llog
LOCAL_LDLIBS 			+= -landroid
LOCAL_LDLIBS 			+= -ljnigraphics
############################ build *.so    ###########################
include $(BUILD_SHARED_LIBRARY)
#################################################################################
