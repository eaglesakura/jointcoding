package com.eaglesakura.jc.framework.context;

import java.util.List;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;

import com.eaglesakura.jc.egl.SurfaceColorSpec;
import com.eaglesakura.jc.egl.WindowDeviceManager;
import com.eaglesakura.jc.egl.view.EGLSurfaceView;
import com.eaglesakura.jc.egl.view.EGLTextureView;
import com.eaglesakura.jc.egl.view.RenderingSurface;
import com.eaglesakura.jc.framework.app.JointApplicationRenderer;
import com.eaglesakura.jc.ui.NativeTouchEvent;
import com.eaglesakura.jc.util.AndroidUtil;
import com.eaglesakura.jcprotocol.SurfacePixelFormatProtocol;
import com.eaglesakura.jcprotocol.framework.JointApplicationProtocol;

/**
 * TextureView1枚でアプリ管理を行うFragmentを構築する
 * iOSとの共通処理を記述するため、ある程度用途を限定する。
 * 
 * 現状では主にゲーム用途として考える。
 */
public class JointApplicationFragment extends Fragment implements WindowDeviceManager.SurfaceListener {

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

    /**
     * サーフェイスが休止状態だったらtrue
     */
    boolean surfaceSuspend = false;

    static final String ARG_RENDERING_CLASS = "ARG_RENDERING_CLASS";

    /**
     * タッチ制御をNativeに伝えるクラス
     */
    private View.OnTouchListener surfaceTouchListener = new View.OnTouchListener() {
        @Override
        public boolean onTouch(View v, MotionEvent event) {
            if (renderer == null) {
                return true;
            }

            List<NativeTouchEvent> nativeEvents = NativeTouchEvent.toNativeEvents(event);
            // 分解して送信する
            for (NativeTouchEvent evt : nativeEvents) {
                renderer.dispatchTouchEvent(evt);
            }
            return true;
        }
    };

    public JointApplicationFragment() {
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // レンダラーを作成する
        renderer = createRenderer();
        if (renderer == null) {
            throw new RuntimeException("Renderer null");
        }
        // レンダラーを初期化する
        renderer.initialize();

        // レンダリングサーフェイスを作成する
        surface = createRenderingView();
        View view = surface.getView();
        view.setOnTouchListener(surfaceTouchListener);
        return view;
    }

    /**
     * サーフェイスの描画準備が完了した
     */
    @Override
    public void onSurfaceInitializeCompleted(WindowDeviceManager device) {
        AndroidUtil.log("onSurfaceInitializeCompleted");
        surfaceSuspend = false;
        renderer.onAppStart();
    }

    /**
     * サーフェイスのサイズが変更された
     */
    @Override
    public void onSurfaceSurfaceSizeChanged(WindowDeviceManager device, int width, int height) {
        AndroidUtil.log("onSurfaceSurfaceSizeChanged");
        surfaceSuspend = false;
        renderer.postSurfaceSize(width, height);
    }

    /**
     * サーフェイスの復旧が行われた
     */
    @Override
    public void onSurfaceRestored(WindowDeviceManager device) {
        AndroidUtil.log("onSurfaceRestored");
        surfaceSuspend = false;
        renderer.postStateChangeRequest(JointApplicationProtocol.State_Running);
        renderer.onAppResume();
    }

    /**
     * サーフェイスの廃棄が開始された
     */
    @Override
    public void onSurfaceDestroyBegin(WindowDeviceManager device) {
        AndroidUtil.log("onSurfaceDestroyBegin");
        if (renderer != null) {
            renderer.postStateChangeRequest(JointApplicationProtocol.State_Paused);
        }
        surfaceSuspend = true;
    }

    /**
     * サーフェイスが休止状態だったらtrueを返す
     * @return
     */
    public boolean isSurfaceSuspend() {
        return surfaceSuspend;
    }

    /**
     * 復帰処理を行う
     */
    @Override
    public void onResume() {
        super.onResume();

        if (!isSurfaceSuspend()) {
            renderer.onAppResume();
        }
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
        destroyAppContexts();
        super.onDestroy();
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
        int[] surfaceSpecs = new int[JointApplicationProtocol.QueryKey_RequestSurfaceSpecs_length];

        // サーフェイスを問い合わせる
        renderer.queryParams(JointApplicationProtocol.QueryKey_RequestSurfaceSpecs, 0, surfaceSpecs);

        RenderingSurface surface = null;
        SurfaceColorSpec color = (surfaceSpecs[0] == SurfacePixelFormatProtocol.RGB8 ? SurfaceColorSpec.RGB8
                : SurfaceColorSpec.RGBA8);
        boolean depth = surfaceSpecs[1] != 0;
        boolean stencil = surfaceSpecs[2] != 0;
        if (surfaceSpecs[3] != 0) {
            AndroidUtil.log("Rendering toTextureView");
            EGLTextureView view = new EGLTextureView(getActivity());
            view.initialize(color, depth, stencil, this);

            surface = view;
        } else {
            AndroidUtil.log("Rendering to SurfaceView");
            EGLSurfaceView view = new EGLSurfaceView(getActivity());
            view.initialize(color, depth, stencil, this);

            surface = view;
        }

        // レンダリングデバイスの設定
        renderer.setWindowDevice(surface.getDevice());
        return surface;
    }

    /**
     * 
     * @param renderingClass
     * @return
     */
    public static JointApplicationFragment createInstance(Class<? extends JointApplicationRenderer> renderingClass) {
        Bundle args = new Bundle();
        args.putSerializable(ARG_RENDERING_CLASS, renderingClass);
        JointApplicationFragment result = new JointApplicationFragment();
        result.setArguments(args);
        return result;
    }

    /**
     * Fragmentクラスを指定して生成する
     * @param fragmentClass
     * @param renderingClass
     * @return
     */
    public static JointApplicationFragment createInstance(Class<JointApplicationFragment> fragmentClass,
            Class<? extends JointApplicationRenderer> renderingClass) {
        try {
            Bundle args = new Bundle();
            args.putSerializable(ARG_RENDERING_CLASS, renderingClass);
            JointApplicationFragment result = fragmentClass.newInstance();
            result.setArguments(args);
            return result;
        } catch (Exception e) {
            return null;
        }
    }
}
