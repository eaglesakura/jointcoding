/**
 * export from "Joint Coding Project"
 */
#ifndef __URIPROTOCOL_H__
#define __URIPROTOCOL_H__
#include "jointcoding.h"
namespace jc {

class UriProtocol {
public:
    virtual ~UriProtocol(void){
    }


    static const ::jc::charactor* SCHEME_APPLI_ASSETS;
    static const ::jc::charactor* SCHEME_LOCALSTRAGE;
    static const ::jc::charactor* SCHEME_EXTERNALSTRAGE;
    static const ::jc::charactor* SCHEME_HTTP;
    static const ::jc::charactor* SCHEME_HTTPS;
};

}

#endif // __URIPROTOCOL_H__
