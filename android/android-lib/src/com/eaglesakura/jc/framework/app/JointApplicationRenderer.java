package com.eaglesakura.jc.framework.app;

import com.eaglesakura.jc.Jointable;
import com.eaglesakura.jc.egl.DeviceManager;
import com.eaglesakura.jc.egl.WindowDeviceManager;
import com.eaglesakura.jc.framework.app.thread.JointThread;
import com.eaglesakura.jc.jni.Pointer;
import com.eaglesakura.jcprotocol.TouchEventProtocol;
import com.eaglesakura.jcprotocol.framework.JointApplicationProtocol;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * TextureView1枚でアプリ管理を行うFragmentを構築する
 * iOSとの共通処理を記述するため、ある程度用途を限定する。
 * 
 * 現状では主にゲーム系のメインループ処理が必要なアプリとして考える。
 */
@JCClass(
         cppNamespace = "ndk")
public abstract class JointApplicationRenderer implements Jointable {
    /**
     * GPU管理クラス
     */
    WindowDeviceManager windowDevice = null;

    /**
     * 排他制御のためのロックオブジェクト
     */
    final Object lock = new Object();

    /**
     * アプリのメインコンテキスト
     * Native側で管理する
     */
    Pointer appContext = null;

    public JointApplicationRenderer() {
    }

    /**
     * 初期化を行う
     * この時点でNative側のクラスが生成され、呼び出し準備を整える
     */
    public void initialize() {
        createNativeContext();
        // 初期化されていなければエラーである
        if (appContext == null) {
            throw new IllegalStateException("appContext == null");
        }
    }

    /**
     * ウィンドウデバイスの設定を行う
     * @param windowDevice
     */
    public void setWindowDevice(WindowDeviceManager windowDevice) {
        this.windowDevice = windowDevice;
    }

    /**
     * アプリの動作を開始する
     */
    public void onAppStart() {
        // 初期化を行わせる
        onNativeInitialize();
    }

    /**
     * アプリの休止を行う
     */
    public void onAppPause() {
        postStateChangeRequest(JointApplicationProtocol.State_Paused);
    }

    /**
     * アプリのレジュームを行う
     */
    public void onAppResume() {
        postStateChangeRequest(JointApplicationProtocol.State_Running);
    }

    /**
     * アプリの廃棄を行う
     */
    public void onAppDestroy() {
        postStateChangeRequest(JointApplicationProtocol.State_Destroyed);

        if (appContext != null) {
            appContext.release();
            appContext = null;
        }
        if (windowDevice != null) {
            windowDevice.dispose();
            windowDevice = null;
        }
    }

    /**
     * レンダリングステートを取得する
     * @return
     */
    public int getRendererState() {
        if (appContext != null) {
            //            int[] param = new int[1];
            String[] param = new String[1];
            //            queryIntParams(JointApplicationProtocol.PostKey_QueryApplicationState, 0, param);
            postParams(JointApplicationProtocol.PostKey_QueryApplicationState, 0, param);
            return Integer.valueOf(param[0]);
        } else {
            return JointApplicationProtocol.State_Destroyed;
        }
    }

    /**
     * 解放済みであったらtrue
     * @return
     */
    public boolean isDestroyed() {
        return getRendererState() == JointApplicationProtocol.State_Destroyed;
    }

    /**
     * ネイティブコンテキストを取り出す
     */
    @Override
    @JCMethod
    public Pointer getNativePointer(int key) {
        if (key == Jointable.KEY_MAINCONTEXT) {
            return appContext;
        }

        return null;
    }

    /**
     * ネイティブコンテキストを設定する
     */
    @Override
    @JCMethod
    public void setNativePointer(int key, Pointer ptr) {
        if (key == Jointable.KEY_MAINCONTEXT) {
            appContext = ptr;
        }
    }

    /**
     * デバイス管理クラスを取得する
     * @return
     */
    @JCMethod
    public final DeviceManager getWindowDevice() {
        return windowDevice;
    }

    /**
     * ウィンドウデバイスのスレイブとなるデバイスを生成する
     * @return
     */
    @JCMethod
    public final DeviceManager createSlaveDevice() {
        return windowDevice.createSlaveDevice();
    }

    /**
     * タッチイベントを処理させる
     * @param event
     */
    @JCMethod(
              nativeMethod = true)
    public final native void dispatchTouchEvent(TouchEventProtocol event);

    /**
     * パラメータのpushを行う
     * Stringで受け渡すため、速度的に不利になるが多くのデータは問題なく渡せるはず
     * @param main_key 主キー。基本的に {@link com.eaglesakura.jcprotocol.framework.JointApplicationProtocol} のQuery_XXXを設定する。その他の拡張は自由
     * @param sub_key サブキー。主キーによっては利用する
     * @param params 書き込み値を格納する。POSTが要求する十分な長さが必要
     * @return
     */
    @JCMethod(
              nativeMethod = true)
    public final native boolean postParams(int main_key, int sub_key, String[] params);

    /**
     * Nativeからのパラメータを受け取る。書き込んだparamsはNativeへ返される。
     * @param main_key
     * @param sub_key
     * @param params
     * @return
     */
    @JCMethod
    public boolean receiveParams(int main_key, int sub_key, String[] params) {
        return false;
    }

    /**
     * サーフェイスサイズの変更通知を行う
     * @param width
     * @param height
     */
    public void postSurfaceSize(int width, int height) {
        // 新しいサーフェイス値の書き込みを行う
        postParams(JointApplicationProtocol.PostKey_SurfaceSize, 0, new String[] {
                String.valueOf(width), String.valueOf(height),
        });
    }

    /**
     * ステート変更リクエストを送る
     * @param JointApplicationProtocol_State
     */
    public final void postStateChangeRequest(int JointApplicationProtocol_State) {
        if (appContext != null) {
            // string実装に変更
            postParams(JointApplicationProtocol.PostKey_StateRequest, 0, new String[] {
                String.valueOf(JointApplicationProtocol_State),
            });
        }
    }

    /**
     * ネイティブ側のメインループ処理を行う
     */
    @JCMethod(
              nativeMethod = true)
    final native void onNativeMainLoop();

    /**
     * ネイティブ側の初期化を行わせる
     */
    @JCMethod(
              nativeMethod = true)
    final native void onNativeInitialize();

    /**
     * 新規タスクの生成を伝える
     * @param taskId
     * @param userData
     */
    @JCMethod(
              nativeMethod = true)
    native void onNativeNewtask(int taskId, int userData);

    /**
     * 新規の処理タスクを生成する
     * @param taskId
     * @param userData
     */
    @JCMethod
    public void startNewtask(final int taskId, final int userData) {

        // スレッド名を決定する
        String name = null;
        if (taskId == JointApplicationProtocol.SystemTask_Mainloop) {
            name = "jctask-mainloop";
        } else {
            name = "jctask-" + String.format("ID(0x%x)", taskId);
        }

        // 新規スレッドを作成する
        final JointThread thread = createThread(name, new JointThread.Task() {
            @Override
            public void run(JointThread thread) {
                // 裏スレッドからタスクをコールバックする
                thread.getRenderer().onNativeNewtask(taskId, userData);
            }
        });
        thread.start();
    }

    protected Pointer retainAppContextPointer() {
        synchronized (lock) {
            Pointer result = appContext;
            if (result != null) {
                result.retain();
            }
            return result;
        }
    }

    /**
     * 操作スレッドを生成する
     * @param slave スレイブデバイス（ロード用デバイス）を生成する場合はtrue、それ以外はレンダリングデバイスを利用する
     * @param threadName
     * @return
     */
    protected JointThread createThread(String threadName, JointThread.Task task) {
        JointThread thread = new JointThread(this, retainAppContextPointer(), task);
        thread.setName(threadName);
        return thread;
    }

    /**
     * Native Contextを作成する。
     * どのようなApplicationがNative側で生成されるかはこの実装に任せる
     */
    protected abstract void createNativeContext();

    /**
     * パラメータ受け取り用String配列を生成する
     * @param length
     * @return
     */
    @JCMethod
    static String[] newStringArray(int length) {
        return new String[length];
    }
}
