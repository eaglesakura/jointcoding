# 設定は共通のmakefileに任せる
# project/jni/Application.mk に配置する。
#################################################################################
## setup
include $(JOINTCODING_HOME)/bin/android-make/types.mk
#################################################################################
APP_MODULES := $(JOINTCODING_LIB_MODULE)
include $(JOINTCODING_HOME)/bin/android-make/jointcoding-Application.mk