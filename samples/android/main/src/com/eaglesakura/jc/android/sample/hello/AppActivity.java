package com.eaglesakura.jc.android.sample.hello;

import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentTransaction;
import android.os.Bundle;

import com.eaglesakura.jc.android.sample.R;
import com.eaglesakura.jc.android.sample.hello.gl.TextureViewFragment;

public class AppActivity extends Activity {

    static {
        System.loadLibrary("jointcoding-lib");
        System.loadLibrary("jointcoding-app");
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.glfragment);

        if (savedInstanceState == null) {
            FragmentTransaction transaction = getFragmentManager().beginTransaction();
            /*
            {
                Fragment fragment = new AppFragment();
                transaction.add(R.id.main_gl_singlewindow, fragment, fragment.getClass().getName());
            }
            */
            {
                Fragment fragment = new TextureViewFragment();
                transaction.add(R.id.main_gl_singlewindow, fragment, fragment.getClass().getSimpleName());
            }
            transaction.commit();
        }
    }

}
