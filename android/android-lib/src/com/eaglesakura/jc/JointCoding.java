package com.eaglesakura.jc;

import com.eaglesakura.jc.framework.context.JointApplication;
import com.eaglesakura.jc.jni.context.NativeContext;

public class JointCoding {

    private JointCoding() {
    }

    /**
     * JointCoding Library初期化を行う
     * @param appContext
     */
    public static void initialize(JointApplication application) {
        {
            String[] files = application.getNativeLibraryNames();
            for (String name : files) {
                System.loadLibrary(name);
            }
        }

        NativeContext.initialize(application);
    }
}
