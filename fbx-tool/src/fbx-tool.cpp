//============================================================================
// Name        : fbx-tool.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "jointcoding.h"

int main(int argv, char *args[]) {
    jclog("hello fbx-tool!!");
    jclogf("args(%d)", argv);
    for( int i = 0; i < argv; ++i ) {
        jclogf("arg[i] = %s", i, args[i]);
    }
	return 0;
}
