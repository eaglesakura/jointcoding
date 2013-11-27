/*
 * JCStringUtil.h
 *
 *  Created on: 2012/06/28
 *      Author: Takeshi
 */

#ifndef JCSTRINGUTIL_H_
#define JCSTRINGUTIL_H_

#include    "jointcoding.h"
#include    <vector>

namespace jc {

/**
 * ストリングを特定の文字をセパレータにして分割する。
 * 参考URL
 * http://d.hatena.ne.jp/sun-rise/20091010/1255136275
 *
 * @param origin 分割元のURL
 * @param delim 分割用の文字列
 * @param result 格納先のベクター
 */
s32 split(const String &origin, const String &delim, ::std::vector<String, StlAllocator<String> > *result);

/**
 * パスを分解し、ファイル名を取り出す。
 * 取り出せなかった場合はそのまま帰す。
 */
const charactor* getFileName(const charactor *path);

/**
 * パスを分解し、ファイル拡張子を取り出す。
 * 取り出せなかった場合、NULLを返す。
 */
const charactor* getFileExt(const charactor *path);

}
#endif /* JCSTRINGUTIL_H_ */
