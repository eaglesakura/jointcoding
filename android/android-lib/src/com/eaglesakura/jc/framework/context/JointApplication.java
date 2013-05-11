package com.eaglesakura.jc.framework.context;

import android.app.Application;

import com.eaglesakura.jc.NativeContext;

public class JointApplication extends Application {

    public JointApplication() {
    }

    @Override
    public void onCreate() {
        super.onCreate();

        NativeContext.getInstance(this);
    }
}
