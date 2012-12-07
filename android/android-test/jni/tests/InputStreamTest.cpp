/*
 * InputStreamTest.cpp
 *
 *  Created on: 2012/07/22
 *
 */

#include    "jointcoding.h"
#include    "jcTestSupport.h"
#include    "jcSmartPtr.h"
#include    "jcUri.h"
#include    "jcPlatform.h"
#include    "jcJpegImageDecoder.h"
#include    "boost/timer.hpp"

#include    "stdlib.h"

using namespace jc;

extern "C" {

TESTFUNCTION void Inputstream_openTest() {
    Uri fileUri = Uri::fromAssets("test.txt");
    _assertNotNull( Platform::getFileSystem() );

    try {
        MInputStream stream = Platform::getFileSystem()->openInputStream(fileUri);
        _assertNotNull( stream );

        jclogf("available = %d", stream->available() );

        jc_sa<u8> temp(new u8[ stream->available() + 1 ]);
        stream->read(temp.get(), stream->available());

        jclogf("text = %s", temp.get());

    } catch( const Exception &e ) {
        jcloge(e);
        _assertTrue(false);
    }
}

TESTFUNCTION void Inputstream_openJpegTest() {
    Uri fileUri = Uri::fromAssets("sample_raw.jpg");

    _assertNotNull( Platform::getFileSystem() );

    try {
        MInputStream stream = Platform::getFileSystem()->openInputStream(fileUri);
        _assertNotNull( stream );

        jclogf("available = %d kb", stream->available() / 1024 );


        boost::timer timer;
        jc_sp<ImageDecoder> decoder(new JpegImageDecoder(stream));
        /*
        decoder->decodeHeader();
        {
            decoder->decodeBegin(PixelFormat_RGB888, 5);
            s32 scan = 0;
            while( (scan = decoder->decode()) != 0 ) {
                jclogf("decode lines = %d", scan);
            }
        }
        */
        jclogf("decode time = %d ms", (int)(timer.elapsed() * 1000));

        jclog("stream reset");
        stream.reset();
        jclog("stream reset finish");

        jclog("decoder reset");
//        decoder.reset();
        jclog("decoder reset finish");
    } catch( const Exception &e ) {
        jcloge(e);
        _assertTrue(false);
    }

    jclog("finish function");
}

}

