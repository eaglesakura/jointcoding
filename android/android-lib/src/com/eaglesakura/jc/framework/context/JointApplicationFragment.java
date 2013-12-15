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
import com.eaglesakura.jc.util.StringArrayMap;
import com.eaglesakura.jcprotocol.SurfacePixelFormatProtocol;
import com.eaglesakura.jcprotocol.framework.JointApplicationProtocol;
import com.eaglesakura.jcprotocol.framework.SurfaceSpecProtocol;

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
        AndroidUtil.log("onResume");
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
        AndroidUtil.log("onPause");
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
        AndroidUtil.log("onDestroy");
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

    public JointApplicationRenderer getRenderer() {
        return renderer;
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
        StringArrayMap map = new StringArrayMap();
        map.put(SurfaceSpecProtocol.KEY_PixelFormat, "");
        map.put(SurfaceSpecProtocol.KEY_HasDepth, "");
        map.put(SurfaceSpecProtocol.KEY_HasStencil, "");
        map.put(SurfaceSpecProtocol.KEY_AndroidTextureView, "");
        map.put(SurfaceSpecProtocol.KEY_AndroidSurfaceViewOnTop, "");

        // サーフェイスを問い合わせる
        {
            String[] specs = map.toArray();
            renderer.postParams(JointApplicationProtocol.PostKey_RequestSurfaceSpecs, 0, specs);
            map.fromArray(specs);
        }

        RenderingSurface surface = null;
        SurfaceColorSpec color = (map.getInteger(SurfaceSpecProtocol.KEY_PixelFormat, 0) == SurfacePixelFormatProtocol.RGB8) ? SurfaceColorSpec.RGB8
                : SurfaceColorSpec.RGBA8;
        boolean depth = map.getBoolean(SurfaceSpecProtocol.KEY_HasDepth, true);
        boolean stencil = map.getBoolean(SurfaceSpecProtocol.KEY_HasStencil, true);

        AndroidUtil.log("Surface / color spec :: " + color.toString());
        AndroidUtil.log("Surface / hasDepth:: " + depth);
        AndroidUtil.log("Surface / hasStencil:: " + stencil);

        // SurfaceView / TextureViewの切り分けを行う
        if (map.getBoolean(SurfaceSpecProtocol.KEY_AndroidTextureView, false)) {
            AndroidUtil.log("Surface / TextureView");
            EGLTextureView view = new EGLTextureView(getActivity());
            view.initialize(color, depth, stencil, this);

            surface = view;
        } else {
            AndroidUtil.log("Surface / SurfaceView");
            EGLSurfaceView view = new EGLSurfaceView(getActivity());
            view.initialize(color, depth, stencil, this);

            // SurfaceViewのトップ設定を行う
            // デフォルトは揺らぎがあるため、必ず設定する
            AndroidUtil.log(String.format("Surface Top(%s)", map.get(SurfaceSpecProtocol.KEY_AndroidSurfaceViewOnTop)));
            view.setZOrderOnTop(map.getBoolean(SurfaceSpecProtocol.KEY_AndroidSurfaceViewOnTop, false));

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
        return createInstance(JointApplicationFragment.class, renderingClass);
    }

    /**
     * Fragmentクラスを指定して生成する
     * @param fragmentClass
     * @param renderingClass
     * @return
     */
    public static JointApplicationFragment createInstance(Class<? extends JointApplicationFragment> fragmentClass,
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
