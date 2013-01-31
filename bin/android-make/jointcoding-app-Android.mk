# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# setup
RELATIVITY_JC_PATH := $(shell ${JOINTCODING_HOME}/bin/abspath2rel.sh ${JOINTCODING_HOME})
############################ shared headers ###########################
# 設定は共通のmakefileに任せる
include $(JOINTCODING_HOME)/bin/android-make/build-includes.mk

############################ make flags     ###########################
include $(JOINTCODING_HOME)/bin/android-make/build-flags.mk
############################ import libs   ###########################
include $(JOINTCODING_HOME)/bin/android-make/import-libs.mk

# シンボルを全て取り込む
LOCAL_LDLIBS +=  $(RELATIVITY_JC_PATH)/android/android-lib/libs/$(TARGET_ARCH_ABI)/lib$(JOINTCODING_LIB_MODULE).so
############################ build *.so    ###########################
include $(BUILD_SHARED_LIBRARY)
################### import cpufeature module  ########################
$(call import-module,cpufeatures)
# -------------------------------- finish  -------------------------------- 