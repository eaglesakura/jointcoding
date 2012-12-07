/*
 * ListTest.cpp
 *
 *  Created on: 2012/08/21
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "jcOrderAccessList.h"

using namespace jc;

extern "C" {

TESTFUNCTION void OrderAccessList_make() {

    const s32 MAX_LOOP = 10;
    {
        OrderAccessList<s32> list;

        {
            for (s32 i = 0; i < MAX_LOOP; ++i) {
                list.add(i * 100);
            }

            OrderAccessList<s32>::Iterator itr = list.iterator();
            _assertEquals(list.size(), MAX_LOOP);

            while (itr->hasNext()) {
                s32 data = itr->next();
                if ((data / 100) % 2) {
                    itr->remove();
                }
                jclogf("data = %d", data);
            }

            list.debug();

            list.clear();
            _assertEquals(list.size(), 0);
        }
    }

    /*
     {
     OrderAccessList<s32> list;

     {
     for (s32 i = 0; i < MAX_LOOP; ++i) {
     list.add(0, i * 100);
     }

     list.debug();
     _assertEquals(list.size(), MAX_LOOP);
     }
     }

     {
     OrderAccessList<s32> list;

     {
     for (s32 i = 0; i < MAX_LOOP; ++i) {
     list.add(i / 2, i * 100);
     }

     list.debug();
     _assertEquals(list.size(), MAX_LOOP);
     }
     }
     */
}

}

