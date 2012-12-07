/*
 * ThreadTest.cpp
 *
 *  Created on: 2012/07/22
 *
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "ndkJavaJointThread.h"
#include    "jcJpegImageDecoder.h"
#include    "boost/timer.hpp"
#include    "jcTimer.h"
#include    "jcBoostThread.h"
#include    "jcLooper.h"
#include    "jcSmartPtr.h"
#include    "jcPlatform.h"

using namespace jc;
using namespace ndk;

extern "C" {

class ThreadRunnable: public Runnable {
    jc::time start;
public:
    virtual void run() {

        for (int i = 0; i < 10; ++i) {
            boost::timer t;
            this->start = jc::Timer::currentTime();
            Thread::sleep(10);
            ThreadID id;
            s32 ms = Timer::lapseTimeMs(start);
            jclogf("thread running = %s", id.toString().c_str());
            jclogf("thread running time = %d ms / %d tms", ms, (s32)(t.elapsed() * 1000));
        }
    }
};

class LooperThreadMain: public Runnable {
public:
    jc_sp<Looper> looper;
    LooperThreadMain() {
        looper.reset(new Looper());
    }

    virtual ~LooperThreadMain() {
        jclog("delete LooperThreadMain");
    }

    virtual void run() {
        {
            ThreadID id;
            jclogf("looper thread id = %s", id.toString().c_str());
        }
        looper->execute();
    }
};

static s32 looper_running = 0;
static s32 looper_created = 0;

class LooperRunner: public Runnable {
    s32 id;
    jc::time created;
public:
    LooperRunner(s32 id ) {
        ++looper_created;
        this->id = id;
        created = jc::Timer::currentTime();
    }

    virtual ~LooperRunner() {
        jclogf("delete LooperRunner %x", this);
    }

    virtual void run() {
        ++looper_running;
        {
            ThreadID id;
            jclogf("runner = %d | delay = %d ms", this->id, Timer::lapseTimeMs(created));
        }
    }
};

class LooperStopRunner : public Runnable {
    jc_wp<Looper> looper;
public:
    LooperStopRunner(jc_sp<Looper> looper) {
        this->looper = looper;
    }

    virtual ~LooperStopRunner() {
        jclogf("delete LooperStopRunner %x", this);
    }

    virtual void run() {
        if( jc_sp<Looper> lock = looper.lock() ) {
            lock->cancel();
        }
    }
};

TESTFUNCTION void Thread_looperTest() {
    {
        ThreadID id;
        jclogf("main thread id = %s", id.toString().c_str());
    }
    jc_sp<LooperThreadMain> runnable(new LooperThreadMain());
    MNativeThread thread(new BoostThread((jc_sp<Runnable>)runnable));

    thread->start();
    runnable->looper->postAtDelay(MRunnable(new LooperRunner(9)), 40);
    runnable->looper->postAtDelay(MRunnable(new LooperRunner(0)), 0);
    runnable->looper->postAtDelay(MRunnable(new LooperRunner(5)), 100);
    runnable->looper->postAtDelay(MRunnable(new LooperRunner(4)), 80);
    runnable->looper->postAtDelay(MRunnable(new LooperRunner(10)), 50);
    runnable->looper->postAtDelay(MRunnable(new LooperRunner(1)), 100);
    runnable->looper->postAtDelay(MRunnable(new LooperRunner(3)), 90);
    runnable->looper->postAtDelay(MRunnable(new LooperRunner(2)), 300);
    runnable->looper->postAtDelay(MRunnable(new LooperStopRunner(runnable->looper)), 600);

    thread->join();

    _assertEquals(looper_running, looper_created);
}

TESTFUNCTION void Thread_boost_createTest() {
    {
        ThreadID id;
        jclogf("test thread id = %s", id.toString().c_str());
    }
    MRunnable runnable(new ThreadRunnable());
    MNativeThread thread(new BoostThread(runnable));

    thread->start();
    thread->join();
    jclogf("test finish %x", thread.get());
}

TESTFUNCTION void Thread_boost_autoreleaseTest() {
    {
        ThreadID id;
        jclogf("test thread id = %s", id.toString().c_str());
    }
    MRunnable runnable(new ThreadRunnable());
    Thread *thread = new BoostThread(runnable);

    thread->autoRelease()->start();
    Thread::sleep(1000 * 5);
    jclogf("test finish %x", thread);
}

TESTFUNCTION void Thread_createTest() {
    {
        ThreadID id;
        jclogf("test thread id = %s", id.toString().c_str());
    }
    MRunnable runnable(new ThreadRunnable());
    MNativeThread thread(new JavaJointThread(runnable));

    thread->start();
    thread->join();
}

TESTFUNCTION void Thread_autoreleaseTest() {
    {
        ThreadID id;
        jclogf("test thread id = %s", id.toString().c_str());
    }
    MRunnable runnable(new ThreadRunnable());
    Thread *thread = new JavaJointThread(runnable);

    thread->autoRelease()->start();
    Thread::sleep(1000 * 5);
    jclog("test finished");
}

TESTFUNCTION void Thread_otherThreadJniCallTest() {
    {
        ThreadID id;
        jclogf("test thread id = %s", id.toString().c_str());
    }

    class LoadRunnable: public Runnable {
    public:
        virtual void run() {
            Uri fileUri = Uri::fromAssets("sample_raw.jpg");

            _assertNotNull( Platform::getFileSystem().get() );

            try {
                MInputStream stream = Platform::getFileSystem()->openInputStream(fileUri);
                _assertNotNull( stream.get() );

                jclogf("available = %d kb", stream->available() / 1024 );

                jc_sp<ImageDecoder> decoder(new JpegImageDecoder(stream));

                decoder->decodeHeader();

                boost::timer timer;

                {
                    decoder->decodeBegin(PixelFormat_RGB888, 5);
                    s32 scan = 0;
                    while( (scan = decoder->decode()) != 0 ) {
                        jclogf("decode lines = %d", scan);
                    }
                }
                jclogf("decode time = %d ms", (int)(timer.elapsed() * 1000));
            } catch( const IOException &ioe) {
                jcloge(ioe);
                _test_failed("IOException file not found??");
            } catch( const Exception &e ) {
                jcloge(e);
                _assertTrue(false);
            }
        }
    };

    MRunnable runnable((Runnable*) new LoadRunnable());
    Thread *thread = new JavaJointThread(runnable);

    thread->autoRelease()->start();
    Thread::sleep(1000 * 1);
    jclog("test finished");
}

}

