//============================================================================
// Name        : fbx-converter.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "jointcoding.h"
#include "jctest/cdt/CdtTestRunner.h"

//#define RUN_TEST

int main(int argv, char *args[]) {
    jclog("hello fbx-tool!!");

    jc::CdtTestRunner::initialize(argv, args);
    {

#ifndef RUN_TEST
        add_unittest(testFunction);
        add_unittest(test_makeFileArchive);
#else
        add_unittest(test_binaryOutput);
        add_unittest(test_matrix_transform);
        add_unittest(test_matrix_rotate);
        add_unittest(test_matrix_scaling);
        add_unittest(test_matrix_multiply);
        add_unittest(test_string_hashMap);
        add_unittest(test_matrix_invert);

        add_unittest(test_fileEnum);
        add_unittest(test_makeFileArchive);
#endif
    }
    jc::CdtTestRunner::run();

//    testFunction();
    return 0;
}
