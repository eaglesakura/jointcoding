# ARM-v7命令.
# おそらくARM現行端末なら問題なく対応しているはず
APP_ABI := armeabi-v7a

# 例外・RTTI対応版のSTLを利用する
APP_STL:= gnustl_static
# APP_STL := stlport_static

APP_MODULES := jcapp