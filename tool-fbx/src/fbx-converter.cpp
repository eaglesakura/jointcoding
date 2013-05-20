//============================================================================
// Name        : fbx-converter.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "jointcoding.h"
#include "jctest/cdt/CdtTestRunner.h"

using namespace jc;

//#define RUN_TEST

int main(int argv, char *args[]) {

//    if (argv < 3) {
//        jclog("arg < 2");
//        return 1;
//    }

    jclog("hello fbx-tool!!");

    if (argv >= 3) {

        jclogf("Convert (%s) -> (%s)", args[0], args[1]);

        extern void fbx2binary(const String &fbx3ds_file, const String &dst_file);
        fbx2binary(args[1], args[2]);
    }

#if 0
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
#endif

//    testFunction();
    return 0;
}
