package com.eaglesakura.jc.benchmark;

import android.app.Activity;
import android.app.Fragment;
import android.app.FragmentTransaction;
import android.os.Bundle;

import com.eaglesakura.jc.benchmark.app.BenchmarkRenderer;
import com.eaglesakura.jc.framework.context.JointApplicationFragment;
import com.eaglesakura.jc.util.AndroidUtil;

public class MainActivity extends Activity {

    static {
        System.loadLibrary("jcapp");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) {
            FragmentTransaction transaction = getFragmentManager().beginTransaction();
            {
                Fragment fragment = JointApplicationFragment.createInstance(BenchmarkRenderer.class);
                transaction.add(R.id.rendering_area, fragment, fragment.getClass().getSimpleName());
            }
            transaction.commit();
        }
        AndroidUtil.log("activity created");
    }

}
