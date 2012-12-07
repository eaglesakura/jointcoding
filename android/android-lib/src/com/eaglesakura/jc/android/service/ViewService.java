package com.eaglesakura.jc.android.service;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.PixelFormat;
import android.os.IBinder;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;

/**
 * View表示を前提としたService
 * 
 *
 */
public abstract class ViewService extends Service {

    /**
     * Service View
     */
    private View serviceView = null;

    /**
     * WindowManager
     */
    private WindowManager windowManager = null;

    /**
     * レイヤーのレイアウトパラメータを取得する
     * @return
     */
    protected WindowManager.LayoutParams onCreateLayoutParams() {
        return new WindowManager.LayoutParams(
        // レイアウトの幅 / 高さ設定
                WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.MATCH_PARENT,
                // レイアウトの挿入位置設定
                // TYPE_SYSTEM_OVERLAYはほぼ最上位に位置して、ロック画面よりも上に表示される。
                // ただし、タッチを拾うことはできない。
                WindowManager.LayoutParams.TYPE_SYSTEM_OVERLAY,
                // ウィンドウ属性
                WindowManager.LayoutParams.FLAG_HARDWARE_ACCELERATED,
                // 透過属性を持たなければならないため、TRANSLUCENTを利用する
                PixelFormat.TRANSLUCENT);
    }

    /**
     * Serviceとして表示するView
     * @return
     */
    protected abstract View onCreateServiceView(LayoutInflater inflater);

    /**
     * IDが一致するViewを探す
     * @param id
     * @return
     */
    protected View findViewById(int id) {
        if (serviceView == null) {
            return null;
        }

        return serviceView.findViewById(id);
    }

    /**
     * View廃棄時に呼び出される
     */
    protected void onDestroyView() {
    }

    @Override
    public void onCreate() {
        super.onCreate();

        windowManager = (WindowManager) getSystemService(Context.WINDOW_SERVICE);

        serviceView = onCreateServiceView(LayoutInflater.from(this));
        if (serviceView == null) {
            throw new NullPointerException("ServiceView is null...");
        }

        // Viewを画面上に重ね合わせする
        windowManager.addView(serviceView, onCreateLayoutParams());
    }

    public WindowManager getWindowManager() {
        return windowManager;
    }

    @Override
    public void onDestroy() {
        {
            onDestroyView();
            windowManager.removeView(serviceView);
        }
        super.onDestroy();
    }

    public View getServiceView() {
        return serviceView;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

}
