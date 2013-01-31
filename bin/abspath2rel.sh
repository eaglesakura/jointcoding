#!/bin/sh
#abspath2rel.sh
# http://d.hatena.ne.jp/anmino/20100309/1268061553

if [ -z "$1" ]; then
    echo "Usage: abspath2rel.sh abs_path"
    exit 1
fi

if [ `expr x"$1" : x'/'` -eq 0 ]; then
    echo "$1: not an absolute path"
    exit 1
fi

org=`expr x"$PWD" : x'/\(.*\)'`
abs=`expr x"$1"   : x'/\(.*\)'`
rel="."
org1=""
abs1=""

while true; do
    #基準ディレクトリ、絶対パスの最初の要素を取り出す
    org1=`expr x"$org" : x'\([^/]*\)'`
    abs1=`expr x"$abs" : x'\([^/]*\)'`

    [ "$org1" != "$abs1" ] && break
    #最初の要素を捨てる
    org=`expr x"$org" : x'[^/]*/\(.*\)'`
    abs=`expr x"$abs" : x'[^/]*/\(.*\)'`
done

if [ -n "$org" ]; then    #基準ディレクトリの要素がまだ残っている場合
    _IFS="$IFS" ; IFS='/'
    for c in $org; do     #必要な階層分遡る
        rel="$rel/.."
    done
    IFS="$_IFS"
fi

if [ -n "$abs" ]; then    #絶対パスの要素が残っている場合
    rel="$rel/$abs"       #相対パスに付け加える
fi

rel=`expr x"$rel" : x'\./\(.*\)'`
echo "$rel"
