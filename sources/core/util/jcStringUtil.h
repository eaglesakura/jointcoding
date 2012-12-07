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
s32 split(String origin, String delim, ::std::vector<String> *result);

}
#endif /* JCSTRINGUTIL_H_ */
