/*
 * jcMemory.cpp
 *
 *  Created on: 2012/12/12
 */

#include    "jc/mem/MemoryUtil.h"
#include    <string.h>

namespace jc {

void zeromemory(void* pointer, u32 size) {
    memset(pointer, 0x00, size);
}

}
