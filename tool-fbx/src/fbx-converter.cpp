//============================================================================
// Name        : fbx-converter.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "jointcoding.h"
#include "jctest/CdtTestRunner.h"

void testFunction();

int main(int argv, char *args[]) {
    jclog("hello fbx-tool!!");

    jc::CdtTestRunner::initialize(argv, args);
    {
        add_unittest(testFunction);
        void test_binaryOutput();   add_unittest(test_binaryOutput);
    }
    jc::CdtTestRunner::run();

//    testFunction();
    return 0;
}
