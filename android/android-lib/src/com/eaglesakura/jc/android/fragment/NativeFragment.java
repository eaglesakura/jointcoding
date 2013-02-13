package com.eaglesakura.jc.android.fragment;

import java.util.UUID;

import android.app.Activity;
import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.eaglesakura.jc.android.app.AndroidUtil;
import com.eaglesakura.jc.android.app.NativeContext;
import com.eaglesakura.jc.android.resource.jni.Pointer;
import com.eaglesakura.jc.android.resource.jni.PointerList;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * Nativeにアクションを渡すためのFragment。
 * NDK側は指定のクラスを継承しておく必要がある。
 * 
 *
 */
@JCClass(
         cppNamespace = "ndk")
public abstract class NativeFragment extends Fragment {

    private PointerList pointers = new PointerList();

    /**
     * FragmentごとにUniqueになるべき値
     */
    private final String uuid = UUID.randomUUID().toString();

    /**
     * 起動IDを保存する
     */
    private static final String SAVE_BOOTING_ID = "SAVE_BOOTING_ID";

    /**
     * 自身のID
     */
    private static final String SAVE_FRAGMENT_ID = "SAVE_FRAGMENT_ID";

    /**
     * 
     */
    private static final String PTR_KEY_NATIVESTATE = "PTR_KEY_NATIVESTATE";

    /**
     * savedInstanceStateをNativeの状態に合わせて復旧する
     * @param bundle
     * @return
     */
    public static Bundle filterSavedInstanceState(Bundle bundle) {

        if (bundle == null) {
            return bundle;
        }
        return bundle;
    }

    /**
     * Nativeのステートが有効ならtrueを返す
     * @return
     */
    private boolean isNativeStateExist() {
        return getNativeStatePointer() != Pointer.NULL;
    }

    private void restoreNativeStatePointer(Bundle bundle) {
        // ポインタのレストア
        if (bundle != null) {

            final String bootId = bundle.getString(SAVE_BOOTING_ID);
            final String fragmentId = bundle.getString(SAVE_FRAGMENT_ID);

            // 起動IDが含まれているなら、チェックを行う
            if (bootId != null) {
                if (!bootId.equals(NativeContext.getBootingId())) {
                    AndroidUtil.log("delete app pointer");
                    // 起動IDが現在と一致しない場合、unloadが行われた可能性がある。
                    // BundleのPointerを信用しない。
                    bundle.clear();
                }
            }

            // fragment idが含まれている
            if (fragmentId != null) {
                if (!fragmentId.equals(uuid)) {
                    AndroidUtil.log("restore fragment id");
                    bundle.clear();
                }
            }

            pointers.restore(bundle);

        }

        // メインポインタが作られていなければ作成する
        if (!isNativeStateExist()) {
            pointers.remove(PTR_KEY_NATIVESTATE);
            AndroidUtil.log("newState()");
            Pointer nativeState = createNativeState();
            if (nativeState == null || nativeState.getNativePointer() == Pointer.NULL) {
                throw new NullPointerException("native state == null");
            }

            pointers.add(PTR_KEY_NATIVESTATE, nativeState);
        }
    }

    /**
     * 
     * @param savedInstanceState
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        savedInstanceState = filterSavedInstanceState(savedInstanceState);
        super.onCreate(savedInstanceState);

        // メインポインタが作られていなければ作成する
        restoreNativeStatePointer(savedInstanceState);

        if (isNativeStateExist()) {
            onCreateCompletedNative(savedInstanceState); // native側の処理            
        }
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        savedInstanceState = filterSavedInstanceState(savedInstanceState);
        super.onActivityCreated(savedInstanceState);
        // メインポインタが作られていなければ作成する
        restoreNativeStatePointer(savedInstanceState);

        if (isNativeStateExist()) {
            onActivityCreatedCompleteNative(savedInstanceState); // native側の処理            
        }
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        savedInstanceState = filterSavedInstanceState(savedInstanceState);
        return super.onCreateView(inflater, container, savedInstanceState);
    }

    @Override
    public void onAttach(Activity activity) {
        super.onAttach(activity);

        // NativeContext指定
        NativeContext.getInstance(activity.getApplicationContext());

        if (isNativeStateExist()) {
            onAttachCompletedNative(activity); // native側の処理            
        }
    }

    @Override
    public void onResume() {
        super.onResume();

        if (isNativeStateExist()) {
            onResumeCompletedNative(); // native側の処理            
        }
    }

    @Override
    public void onPause() {
        if (isNativeStateExist()) {
            onPauseBeginNative(); // native側の処理            
        }
        super.onPause();
    }

    @Override
    public void onDetach() {
        if (isNativeStateExist()) {
            onDetachBeginNative();
        }
        super.onDetach();
    }

    @Override
    public void onDestroy() {
        if (isNativeStateExist()) {
            onDestroyBeginNative();
        }
        pointers.clear();
        super.onDestroy();
    }

    /**
     * ポインタ一覧を取得する
     * @return
     */
    protected PointerList getPointers() {
        return pointers;
    }

    @Override
    public void onSaveInstanceState(Bundle outState) {
        AndroidUtil.log("onSaveInstanceState");
        super.onSaveInstanceState(outState);
        // 現在の起動Idを記録する
        outState.putString(SAVE_BOOTING_ID, NativeContext.getBootingId());

        // 現在のFragmentIDを保存
        outState.putString(SAVE_FRAGMENT_ID, uuid);

        // Pointer一覧を記録
        pointers.save(outState);
    }

    @JCMethod(
              nativeMethod = true)
    final native void onCreateCompletedNative(Bundle bundle);

    @JCMethod(
              nativeMethod = true)
    final native void onActivityCreatedCompleteNative(Bundle bundle);

    @JCMethod(
              nativeMethod = true)
    final native void onAttachCompletedNative(Activity activity);

    @JCMethod(
              nativeMethod = true)
    final native void onResumeCompletedNative();

    @JCMethod(
              nativeMethod = true)
    final native void onPauseBeginNative();

    @JCMethod(
              nativeMethod = true)
    final native void onDestroyBeginNative();

    @JCMethod(
              nativeMethod = true)
    final native void onDetachBeginNative();

    /**
     * メインになるNative Pointerを取得する
     * @return
     */
    @JCMethod
    protected int getNativeStatePointer() {
        return pointers.getNativePointer(PTR_KEY_NATIVESTATE);
    }

    /**
     * Native側のPointerを作成させる。
     * @return
     */
    protected abstract Pointer createNativeState();
}
