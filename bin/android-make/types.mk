############################ module names ###########################
# ライブラリ用モジュール
JOINTCODING_LIB_MODULE := jointcoding-lib
# テストサポート用モジュール
JOINTCODING_TESTSUPPORT_MODULE := jointcoding-testsupport

# 起動するAPKのモジュール
JOINTCODING_APP_MODULE := jointcoding-app

# SOに分割しない、フルビルド用のモジュール
JOINTCODING_FULLAPP_MODULE := jointcoding

# テスト用APKのモジュール
JOINTCODING_APPTEST_MODULE := jointcoding-apptest

# setup
RELATIVITY_JC_PATH := $(shell ${JOINTCODING_HOME}/bin/abspath2rel.sh ${JOINTCODING_HOME})