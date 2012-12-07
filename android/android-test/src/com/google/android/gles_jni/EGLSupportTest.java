package com.google.android.gles_jni;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLDisplay;

import android.test.suitebuilder.annotation.SmallTest;

import com.eaglesakura.jc.android.testsupport.JniTestCase;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;

@JCClass(
         cppNamespace = "ndk")
public class EGLSupportTest extends JniTestCase {

    @SmallTest
    public void test_eglConfigNewInstance() {
        Object instance = EGLSupport.newSupportClass(EGLSupport.CLASS_EGLCONFIG_IMPL, 0);
        assertNotNull(instance);
        assertTrue(instance instanceof EGLConfig);
    }

    @SmallTest
    public void test_eglDisplayNewInstance() {
        Object instance = EGLSupport.newSupportClass(EGLSupport.CLASS_EGLDISPLAY_IMPL, 0);
        assertNotNull(instance);
        assertTrue(instance instanceof EGLDisplay);
    }
}
