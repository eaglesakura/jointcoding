package com.eaglesakura.jc.framework.context;

import android.app.Application;

import com.eaglesakura.jc.JointCoding;
import com.eaglesakura.jc.R;

public class JointApplication extends Application {

    public JointApplication() {
    }

    @Override
    public void onCreate() {
        super.onCreate();
        JointCoding.initialize(this);
    }

    /**
     * ビルド済みSOファイル名を取得する
     * @return
     */
    public String[] getNativeLibraryNames() {
        return new String[] {
            getString(R.string.sofile_name)
        };
    }
}
