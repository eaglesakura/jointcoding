package com.eaglesakura.jc.android.sample.service;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

public class ServiceBootActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        startService(new Intent(this, GLViewService.class));
        finish();
    }
}
