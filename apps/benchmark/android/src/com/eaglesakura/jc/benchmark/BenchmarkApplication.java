package com.eaglesakura.jc.benchmark;

import com.eaglesakura.jc.framework.app.JointApplication;

public class BenchmarkApplication extends JointApplication {

    static {
        System.loadLibrary("jcapp");
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }
}
