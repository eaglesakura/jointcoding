package com.eaglesakura.jc.android.sample.service;

import android.graphics.PixelFormat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;

import com.eaglesakura.jc.android.sample.R;
import com.eaglesakura.jc.android.service.ViewService;
import com.eaglesakura.jc.android.thread.UIHandler;
import com.eaglesakura.jc.android.view.GLNativeTextureView;

public class GLViewService extends ViewService {
    /**
     * 表示領域のサイズを設定する
     */
    static final int VIEW_SIZE = 200;

    static final String TAG = GLViewService.class.getSimpleName();
    static {
        System.loadLibrary("jointcoding-lib");
        System.loadLibrary("jointcoding-app");
    }

    /**
     * レイヤーのレイアウトパラメータを取得する
     * @return
     */
    @Override
    protected WindowManager.LayoutParams onCreateLayoutParams() {
        WindowManager.LayoutParams result = new WindowManager.LayoutParams(
        // レイアウトの幅 / 高さ設定
        //                WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.MATCH_PARENT,
                VIEW_SIZE, VIEW_SIZE,
                // レイアウトの挿入位置設定
                // TYPE_SYSTEM_OVERLAYはほぼ最上位に位置して、ロック画面よりも上に表示される。
                // ただし、タッチを拾うことはできない。
                WindowManager.LayoutParams.TYPE_PHONE,
                // ウィンドウ属性
                WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED
                        | WindowManager.LayoutParams.FLAG_WATCH_OUTSIDE_TOUCH
                        | WindowManager.LayoutParams.FLAG_SPLIT_TOUCH | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
                        | WindowManager.LayoutParams.FLAG_LAYOUT_NO_LIMITS,
                // 透過属性を持たなければならないため、TRANSLUCENTを利用する
                PixelFormat.TRANSLUCENT);
        //        result.flags = Gravity.TOP;
        return result;
    }

    @Override
    protected View onCreateServiceView(LayoutInflater inflater) {
        View view = inflater.inflate(R.layout.glservice, null);
        GLNativeTextureView result = (GLNativeTextureView) view.findViewById(R.id.textureview);
        //        result.setFocusable(false);
        result.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                Log.i(TAG, "touchEvent");
                return false;
            }
        });
        result.initialize(0xFFFFFFFF, new GLNativeTextureView.GLES2Callback() {
            @Override
            public void onEGLSurfaceSizeChanged(GLNativeTextureView view, int width, int height) {
            }

            @Override
            public void onEGLResumeCompleted(GLNativeTextureView view) {
            }

            @Override
            public void onEGLPauseCompleted(GLNativeTextureView view) {
            }

            @Override
            public void onEGLPauseBegin(GLNativeTextureView view) {
            }

            @Override
            public void onEGLInitializeCompleted(GLNativeTextureView view) {

            }
        });

        return view;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        UIHandler.postUI(new Runnable() {
            int move = 0;

            @Override
            public void run() {
                WindowManager.LayoutParams param = (WindowManager.LayoutParams) getServiceView().getLayoutParams();
                param.x += 2;
                getWindowManager().updateViewLayout(getServiceView(), param);
                ++move;
                if (move < 1000) {
                    UIHandler.postDelayedUI(this, 1);
                }
            }
        });

        UIHandler.postDelayedUI(new Runnable() {
            @Override
            public void run() {
                stopSelf();
            }
        }, 1000 * 30);
    }

    @Override
    protected void onDestroyView() {
        ((GLNativeTextureView) findViewById(R.id.textureview)).destroy();
    }
}
