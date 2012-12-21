//============================================================================
// Name        : fbx-converter.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "jointcoding.h"
#include "jctest/CdtTestRunner.h"


int main(int argv, char *args[]) {
    jclog("hello fbx-tool!!");

    jc::CdtTestRunner::initialize(argv, args);
    {
        add_unittest(testFunction);
        add_unittest(test_binaryOutput);
        add_unittest(test_matrix_transform);
        add_unittest(test_matrix_rotate);
        add_unittest(test_matrix_scaling);
        add_unittest(test_matrix_multiply);
    }
    jc::CdtTestRunner::run();

//    testFunction();
    return 0;
}
