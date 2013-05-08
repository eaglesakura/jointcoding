package com.eaglesakura.jc.framework.app;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import com.eaglesakura.jc.android.egl.SurfaceColorSpec;
import com.eaglesakura.jc.android.egl.view.EGLTextureView;

/**
 * TextureView1枚でアプリ管理を行うFragmentを構築する
 * iOSとの共通処理を記述するため、ある程度用途を限定する。
 * 
 * 現状では主にゲーム用途として考える。
 */
public abstract class NativeApplicationFragment extends Fragment {

    /**
     * 排他制御のためのロックオブジェクト
     */
    final Object lock = new Object();

    /**
     * レンダリングサーフェイス
     */
    EGLTextureView surface = null;

    /**
     * レンダラークラス
     */
    JointApplicationRenderer renderer = null;

    /**
     * タッチ制御をNativeに伝えるクラス
     */
    private View.OnTouchListener surfaceTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            return true;
        }
    };

    public NativeApplicationFragment() {
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        renderer = createRenderer();
        if (renderer == null) {
            throw new RuntimeException("Renderer null");
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        surface = new EGLTextureView(getActivity());
        surface.initialize(SurfaceColorSpec.RGB8, false, false, renderer);
        surface.setOnTouchListener(surfaceTouchListener);
        return surface;
    }

    /**
     * 復帰処理を行う
     */
    @Override
    public void onResume() {
        super.onResume();
        renderer.onAppResume();
    }

    /**
     * 休止処理を行う
     */
    @Override
    public void onPause() {
        renderer.onAppPause();
        super.onPause();
    }

    /**
     * 廃棄を行う
     */
    @Override
    public void onDestroy() {
        renderer.onAppDestroy();
        super.onDestroy();
    }

    /**
     * レンダリングサーフェイスを取得する
     * @return
     */
    public EGLTextureView getSurface() {
        return surface;
    }

    /**
     * レンダラーを作成する
     * @return
     */
    protected abstract JointApplicationRenderer createRenderer();
}
