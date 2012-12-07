package com.eaglesakura.jc.android.sample.hello;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.eaglesakura.jc.android.fragment.NativeFragment;
import com.eaglesakura.jc.android.resource.jni.Pointer;
import com.eaglesakura.jc.android.resource.jni.Pointer.Mode;
import com.eaglesakura.jc.android.view.GLNativeSurfaceView;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * アプリ用のFragment
 *
 */
@JCClass(
         cppNamespace = "app")
public class AppFragment extends NativeFragment implements GLNativeSurfaceView.GLES2Callback {

    GLNativeSurfaceView glView = null;

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        savedInstanceState = filterSavedInstanceState(savedInstanceState);

        glView = new GLNativeSurfaceView(getActivity());
        glView.initialize(GLNativeSurfaceView.EGL_DEPTHBUFFER_ENABLE, this);
        return glView;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();

        // Viewを廃棄する
        if (glView != null) {
            glView.destroy();
            glView = null;
        }
    }

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLResumeCompleted(GLNativeSurfaceView view);

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLPauseCompleted(GLNativeSurfaceView view);

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLPauseBegin(GLNativeSurfaceView view);

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLInitializeCompleted(GLNativeSurfaceView view);

    @Override
    protected Pointer createNativeState() {
        return new Pointer(newState(), Mode.SharedObject);
    }

    @JCMethod(
              nativeMethod = true)
    native int newState();
}
