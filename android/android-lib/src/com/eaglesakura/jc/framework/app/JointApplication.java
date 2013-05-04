package com.eaglesakura.jc.framework.app;

import android.app.Application;

import com.eaglesakura.jc.android.app.NativeContext;

public class JointApplication extends Application {

    public JointApplication() {
    }

    @Override
    public void onCreate() {
        super.onCreate();

        NativeContext.getInstance(this);
    }
}
