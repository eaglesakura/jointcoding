############################ make flags     ###########################
# 例外を有効
LOCAL_CPPFLAGS   += -fexceptions
# スレッド有効
LOCAL_CPPFLAGS   += -pthread
# RTTIを有効
LOCAL_CPPFLAGS   += -frtti
# 警告無効？
LOCAL_CFLAGS     += -Wno-psabi
# マクロ定義
LOCAL_CPPFLAGS   += -D__ARM_V7__ 