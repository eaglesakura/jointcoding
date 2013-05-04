package com.eaglesakura.jc.benchmark;

import android.app.Activity;
import android.os.Bundle;

import com.eaglesakura.jc.android.app.AndroidUtil;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        AndroidUtil.log("activity created");
    }

}
