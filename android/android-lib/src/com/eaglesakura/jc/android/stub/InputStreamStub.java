package com.eaglesakura.jc.android.stub;

import java.io.IOException;
import java.io.InputStream;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

@JCClass(
         aliasClassName = "java.io.InputStream",
         cppNamespace = "ndk",
         className = "JniInputStream")
public class InputStreamStub extends InputStream {

    @JCMethod
    @Override
    public int read() throws IOException {
        return 0;
    }

    @JCMethod
    @Override
    public int read(byte[] buffer) throws IOException {
        return super.read(buffer);
    }

    @JCMethod
    @Override
    public int read(byte[] buffer, int offset, int length) throws IOException {
        return super.read(buffer, offset, length);
    }

    @JCMethod
    @Override
    public long skip(long byteCount) throws IOException {
        return super.skip(byteCount);
    }

    @JCMethod
    @Override
    public int available() throws IOException {
        return super.available();
    }

    @JCMethod
    @Override
    public void close() throws IOException {
        super.close();
    }
}
