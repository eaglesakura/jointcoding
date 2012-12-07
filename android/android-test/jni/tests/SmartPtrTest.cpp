/*
 * SmartPtr.cpp
 *
 *  Created on: 2012/07/17
 *
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "boost/smart_ptr/shared_ptr.hpp"
#include    "jcSmartPtr.h"

using namespace jc;

extern "C" {

static s32 makeClass = 0;
static s32 destroyClass = 0;

class TestProtocol {
public:
    TestProtocol() {
    }

    virtual ~TestProtocol() {
    }

    virtual void callProtocol() = 0;
};

class TestObject: public Object, public TestProtocol {
public:
    TestObject() {
        jclog("create object!!");
        ++makeClass;
    }

    virtual ~TestObject() {
        jclog("on destroy!!");
        ++destroyClass;
    }

    virtual void callProtocol() {
        jclog("call protocol!!");
    }
};

static void initialize() {
    makeClass = 0;
    destroyClass = 0;
}

/**
 * 正常にコンストラクタ・デストラクタが自動的に呼ばれることを検証する
 */TESTFUNCTION void SmartPtr_ref_Test() {
    // 変数初期化
    initialize();

    /*
    TestObject *obj = new TestObject();
    jc_sp<TestObject> ptr(obj);

    // これでデストラクタが呼ばれるはず
    ptr.reset();

    // make == destroyでないといけない
    _assertTrue(makeClass == 1);
    _assertTrue(destroyClass == 1);
    */
}

/**
 * 正常に代入ができることを検証する
 */TESTFUNCTION void SmartPtr_set_Test() {
    // 変数初期化
    initialize();

    {
        jc_sp<TestObject> ptr(new TestObject());
        jc_sp<TestObject> ptr2 = ptr;
    }

    // make == destroyでないといけない
    _assertEquals(destroyClass, makeClass);
}

void protocolFunc(boost::shared_ptr<TestProtocol> protocol) {
    protocol->callProtocol();
}

/**
 * boost版スマートポインタとプロトコルが正常に動作することを検証する。
 */TESTFUNCTION void SmartPtr_boost_set_Test() {
    // 変数初期化
    initialize();
    jclog("test start");
    boost::shared_ptr<TestProtocol> ptr3;
    {
        boost::shared_ptr<TestObject> ptr(new TestObject());
        boost::shared_ptr<TestProtocol> ptr2 = ptr;
        ptr3 = ptr2;

        jclogf("TestObject-ptr = %x", ptr.get());
        jclogf("Protocol-ptr = %x", ptr2.get());

        protocolFunc(ptr);
        protocolFunc(ptr2);
        ptr2->callProtocol();
    }
    jclog("final protocol call");
    ptr3->callProtocol();
    jclog("destroy protocol");
    ptr3.reset();
    jclog("destroy protocol - finish");

    // make == destroyでないといけない
    _assertEquals(destroyClass, makeClass);
}

/**
 * voidポインタでのdeleteチェック
 */TESTFUNCTION void SmartPtr_boost_voidPtr_Test() {
    initialize();

    boost::shared_ptr<void> voidPtr;
    voidPtr.reset(new TestObject());
    jclogf("pointer = %x", voidPtr.get());
    voidPtr.reset();
    // make == destroyでないといけない
    _assertEquals(destroyClass, makeClass);
}

/**
 * voidプロトコルでのdeleteチェック
 */TESTFUNCTION void SmartPtr_boost_voidProtocol_Test() {
    initialize();

    boost::shared_ptr<void> voidPtr;
    voidPtr.reset((TestProtocol*) new TestObject());
    jclogf("pointer = %x", voidPtr.get());
    voidPtr.reset();
    // make == destroyでないといけない
    _assertEquals(destroyClass, makeClass);
}


/**
 * voidプロトコルでのdeleteチェック
 */TESTFUNCTION void SmartPtr_boost_voidProtocol_set_Test() {
    initialize();
    {
        jc_sp<TestObject> testObject(new TestObject());
        jc_sp<void> voidPtr = testObject;
        jc_sp<TestProtocol> testProtocol = testObject;
        voidPtr = testProtocol;
        jclogf("void ptr = %x", voidPtr.get());
        jclogf("protocol = %x", testProtocol.get());
        testProtocol->callProtocol();
    }
    // make == destroyでないといけない
    _assertEquals(destroyClass, makeClass);
}

}

