package com.eaglesakura.jc.egl.view;

import android.view.View;

import com.eaglesakura.jc.egl.DeviceManager;

public interface RenderingSurface {
    /**
     * 生成済みのデバイスを取得する
     * @return
     */
    DeviceManager getDevice();

    /**
     * サーフェイスを解放する
     */
    void dispose();

    /**
     * View本体を取得する
     * 基本的にthisを想定している
     * @return
     */
    View getView();
}
