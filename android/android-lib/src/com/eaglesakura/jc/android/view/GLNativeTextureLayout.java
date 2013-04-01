package com.eaglesakura.jc.android.view;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.eaglesakura.jc.android.app.AndroidUtil;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;

@JCClass
public class GLNativeTextureLayout extends FrameLayout {
    /**
     * レンダリングサーフェイス一覧
     */
    List<GLNativeTextureView> surfaces = new ArrayList<GLNativeTextureView>();

    public GLNativeTextureLayout(Context context) {
        super(context);
    }

    public GLNativeTextureLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public GLNativeTextureLayout(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    private void _listSurfaces(ViewGroup group) {
        for (int i = 0; i < group.getChildCount(); ++i) {
            View child = group.getChildAt(i);
            if (child instanceof GLNativeTextureView) {
                surfaces.add((GLNativeTextureView) child);
            } else if (child instanceof ViewGroup) {
                _listSurfaces((ViewGroup) child);
            }
        }
    }

    /**
     * サーフェイスをリストアップする
     */
    public void listSurfaces() {
        surfaces.clear();
        for (int i = 0; i < getChildCount(); ++i) {
            View child = getChildAt(i);
            if (child instanceof GLNativeTextureView) {
                surfaces.add((GLNativeTextureView) child);
            } else if (child instanceof ViewGroup) {
                _listSurfaces((ViewGroup) child);
            }
        }

        AndroidUtil.log(String.format("TextureView num(%d)", surfaces.size()));
    }

    /**
     * レンダリングを開始したらその他のスレッドの挙動を止める
     */
    @Override
    protected void dispatchDraw(Canvas canvas) {
        try {
            for (GLNativeTextureView view : surfaces) {
                view.lockEGL();
            }

            super.dispatchDraw(canvas);
        } finally {
            for (GLNativeTextureView view : surfaces) {
                view.unlockEGL();
            }

        }
    }

    //    @Override
    //    public boolean dispatchTouchEvent(MotionEvent ev) {
    //        try {
    //            for (GLNativeTextureView view : surfaces) {
    //                view.lockEGL();
    //            }
    //            return super.dispatchTouchEvent(ev);
    //        } finally {
    //            for (GLNativeTextureView view : surfaces) {
    //                view.unlockEGL();
    //            }
    //        }
    //    }
    //
    //    @Override
    //    public boolean dispatchKeyEvent(KeyEvent event) {
    //        try {
    //            for (GLNativeTextureView view : surfaces) {
    //                view.lockEGL();
    //            }
    //            return super.dispatchKeyEvent(event);
    //        } finally {
    //            for (GLNativeTextureView view : surfaces) {
    //                view.unlockEGL();
    //            }
    //        }
    //    }
}
