/*
 * RAM.cpp
 *
 *  Created on: 2013/04/08
 */

#include    "RAM.h"
#include    "linux/kernel.h"
#include    "sys/resource.h"
#include    "sys/sysinfo.h"

using namespace jc;

namespace ndk {

namespace {

/**
 * カーネル情報
 */
static struct sysinfo kernel_sysinfo;

static jcboolean kernel_updated = jcfalse;

/**
 * プロセスの利用量
 */
static struct rusage usage;

static void update_meminfo() {
    if (!kernel_updated) {
        sysinfo(&kernel_sysinfo);
        kernel_updated = jctrue;
    }
}

}


/**
 * RAMのプロセス合計量(bytes)
 */
jc::u64 RAM::totalMemoryBytes() {
    update_meminfo();
    return kernel_sysinfo.totalram;
}

/**
 * RAMのプロセス使用量(bytes)
 */
jc::u64 RAM::useRssMemoryBytes() {
    getrusage(RUSAGE_SELF, &usage);
    return (u64)usage.ru_maxrss * 1024;
}
}
