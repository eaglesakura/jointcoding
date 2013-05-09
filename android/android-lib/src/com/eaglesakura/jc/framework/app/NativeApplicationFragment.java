package com.eaglesakura.jc.framework.app;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import com.eaglesakura.jc.android.egl.SurfaceColorSpec;
import com.eaglesakura.jc.android.egl.view.EGLSurfaceView;
import com.eaglesakura.jc.android.egl.view.RenderingSurface;

/**
 * TextureView1枚でアプリ管理を行うFragmentを構築する
 * iOSとの共通処理を記述するため、ある程度用途を限定する。
 * 
 * 現状では主にゲーム用途として考える。
 */
public class NativeApplicationFragment extends Fragment {

    /**
     * 排他制御のためのロックオブジェクト
     */
    final Object lock = new Object();

    /**
     * レンダリングサーフェイス
     */
    RenderingSurface surface = null;

    /**
     * レンダラークラス
     */
    JointApplicationRenderer renderer = null;

    static final String ARG_RENDERING_CLASS = "ARG_RENDERING_CLASS";

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
        surface = createRenderingView();

        View view = surface.getView();
        view.setOnTouchListener(surfaceTouchListener);
        return view;
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

        // 明示的に廃棄を行う
        if (getActivity().isFinishing() || isDetached()) {
            destroyAppContexts();
        }

        super.onPause();
    }

    /**
     * 廃棄を行う
     */
    @Override
    public void onDestroy() {
        super.onDestroy();
        destroyAppContexts();
    }

    /**
     * アプリ資源の廃棄を行わせる
     */
    protected void destroyAppContexts() {
        if (renderer != null) {
            renderer.onAppDestroy();
            renderer = null;
        }

        if (surface != null) {
            surface.dispose();
            surface = null;
        }
    }

    /**
     * レンダラーを作成する
     * @return
     */
    protected JointApplicationRenderer createRenderer() {
        try {
            // bundleからclassを取り出す
            @SuppressWarnings("unchecked")
            Class<? extends JointApplicationRenderer> renderingClass = (Class<? extends JointApplicationRenderer>) getArguments()
                    .getSerializable(ARG_RENDERING_CLASS);
            return renderingClass.newInstance();
        } catch (Exception e) {
            throw new IllegalArgumentException("createRenderer newInstance() failed");
        }
    }

    /**
     * レンダリング用のViewを生成する
     * 基本的にはTextureViewを生成する
     * @return
     */
    protected RenderingSurface createRenderingView() {
        //        EGLTextureView result = new EGLTextureView(getActivity());
        EGLSurfaceView result = new EGLSurfaceView(getActivity());
        result.initialize(SurfaceColorSpec.RGB8, true, true, renderer);
        return result;
    }

    /**
     * 
     * @param renderingClass
     * @return
     */
    public static NativeApplicationFragment createInstance(Class<? extends JointApplicationRenderer> renderingClass) {
        Bundle args = new Bundle();
        args.putSerializable(ARG_RENDERING_CLASS, renderingClass);

        NativeApplicationFragment result = new NativeApplicationFragment();
        result.setArguments(args);
        return result;
    }
}
