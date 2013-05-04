package com.eaglesakura.jc.benchmark.app;

import com.eaglesakura.jc.android.view.GLNativeTextureView;
import com.eaglesakura.jc.framework.app.NativeApplicationFragment;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;

@JCClass(
         cppNamespace = "es")
public class BenchmarkFragment extends NativeApplicationFragment {

    public BenchmarkFragment() {
    }

    /**
     * app contextを作成する
     */
    @Override
    protected native void createNativeContext(GLNativeTextureView surface);

}
