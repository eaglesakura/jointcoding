package com.eaglesakura.jc.benchmark.app;

import com.eaglesakura.jc.framework.app.JointApplicationRenderer;
import com.eaglesakura.jc.framework.app.NativeApplicationFragment;

public class BenchmarkFragment extends NativeApplicationFragment {

    public BenchmarkFragment() {
    }

    @Override
    protected JointApplicationRenderer createRenderer() {
        return new BenchmarkRenderer();
    }
}
