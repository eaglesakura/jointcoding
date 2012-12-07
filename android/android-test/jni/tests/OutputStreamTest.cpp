/*
 * OutputStreamTest.cpp
 *
 *  Created on: 2012/07/25
 *
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "jcSmartPtr.h"
#include    "jcUri.h"
#include    "jcJpegImageDecoder.h"
#include    "jcMemoryBlock.h"

using namespace jc;

extern "C" {

TESTFUNCTION void Outputstream_memwriteTest() {

    jc_sp<MemoryBlock> memory(new MemoryBlock(1024));

    const int MAX_LOOP = 1024;
    const int BUFFER_SIZE = 5;
    for (int i = 0; i < MAX_LOOP; ++i) {
        u8 data[BUFFER_SIZE] = {0, 1, 2, 3, 4};
        memory->pushBack(data, 5);
    }

    _assertEquals(memory->getLength(), (MAX_LOOP * BUFFER_SIZE));

    // 中身が正しいことを検証する
    for( int i = 0; i < memory->getLength(); ++i ) {
//        jclogf("memory[%d] = %d", i, (int)memory->getBuffer()[i]);
        _assertEquals( memory->getBuffer()[i], (i % BUFFER_SIZE) );//
    }
}

}

