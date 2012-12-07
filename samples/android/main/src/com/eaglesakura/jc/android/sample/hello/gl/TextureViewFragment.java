package com.eaglesakura.jc.android.sample.hello.gl;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.eaglesakura.jc.android.fragment.NativeFragment;
import com.eaglesakura.jc.android.resource.jni.Pointer;
import com.eaglesakura.jc.android.resource.jni.Pointer.Mode;
import com.eaglesakura.jc.android.view.GLNativeTextureView;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * 
 */
@JCClass(
         cppNamespace = "app")
public class TextureViewFragment extends NativeFragment implements GLNativeTextureView.GLES2Callback {

    GLNativeTextureView glView = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        glView = new GLNativeTextureView(getActivity());
        glView.initialize(0xFFFFFFFF, this);
        return glView;
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        glView.destroy();
        glView = null;
    }

    @Override
    protected Pointer createNativeState() {
        return new Pointer(newState(), Mode.SharedObject);
    }

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLInitializeCompleted(GLNativeTextureView view);

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLSurfaceSizeChanged(GLNativeTextureView view, int width, int height);

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLResumeCompleted(GLNativeTextureView view);

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLPauseBegin(GLNativeTextureView view);

    @JCMethod(
              nativeMethod = true)
    @Override
    public native void onEGLPauseCompleted(GLNativeTextureView view);

    @JCMethod(
              nativeMethod = true)
    native int newState();
}
