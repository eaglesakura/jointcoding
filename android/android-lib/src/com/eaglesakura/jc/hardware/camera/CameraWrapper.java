package com.eaglesakura.jc.hardware.camera;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.util.List;

import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;

import com.eaglesakura.jc.util.AndroidUtil;
import com.eaglesakura.jcprotocol.CameraDeviceProtocol;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * ハードウェアカメラ情報をラッピングする
 */
@JCClass(
         cppNamespace = "ndk")
public class CameraWrapper {

    final Object lock = new Object();

    Camera camera = null;

    /**
     * カメラタイプ
     */
    int CameraDeviceProtocol_TYPE;

    /**
     * 現在のフォーカス状態
     */
    int CameraDeviceProtocol_FOCUSMODE = CameraDeviceProtocol.FOCUSMODE_NONE;

    /**
     * プレビュー用サーフェイス
     */
    PreviewSurface previewSurface = null;

    /**
     * テクスチャ行列
     */
    FloatBuffer textureMatrix = null;

    /**
     * カメラ用パラメーター
     */
    Parameters cameraParams = null;

    public CameraWrapper(int CameraDeviceProtocol_TYPE) {
        this.CameraDeviceProtocol_TYPE = CameraDeviceProtocol_TYPE;
    }

    /**
     * カメラを開く
     * @return
     */
    boolean open() {
        try {
            final int numberOfCameras = Camera.getNumberOfCameras();

            if (CameraDeviceProtocol_TYPE == CameraDeviceProtocol.TYPE_MAIN || numberOfCameras == 1) {
                camera = Camera.open(0);
            } else if (numberOfCameras > 1 && CameraDeviceProtocol_TYPE == CameraDeviceProtocol.TYPE_FRONT) {
                camera = Camera.open(1);
            } else {
                camera = Camera.open();
            }

            if (camera == null) {
                return false;
            }

            cameraParams = camera.getParameters();

            return true;
        } catch (Exception e) {
            AndroidUtil.log(e);
            return false;
        }
    }

    /**
     * カメラの回転角を設定する
     * @param CameraDeviceProtocol_ORIENTATION
     */
    @JCMethod
    public boolean requestOrientation(int CameraDeviceProtocol_ORIENTATION) {
        try {
            int degree = 0;
            switch (CameraDeviceProtocol_ORIENTATION) {
                case CameraDeviceProtocol.ORIENTATION_ROTATE_0:
                    degree = 0;
                    break;
                case CameraDeviceProtocol.ORIENTATION_ROTATE_90:
                    degree = 90;
                    break;
                case CameraDeviceProtocol.ORIENTATION_ROTATE_180:
                    degree = 180;
                    break;
                case CameraDeviceProtocol.ORIENTATION_ROTATE_270:
                    degree = 270;
                    break;
            }

            // 回転角を設定する
            camera.setDisplayOrientation(degree);

            return true;
        } catch (Exception e) {
            AndroidUtil.log(e);
            return false;
        }
    }

    /**
     * オートフォーカス処理中であればtrue
     * @return
     */
    @JCMethod
    public boolean isAutofocusProcessing() {
        return CameraDeviceProtocol_FOCUSMODE == CameraDeviceProtocol.FOCUSMODE_PROCESSING;
    }

    /**
     * オートフォーカスを開始する
     * @return
     */
    @JCMethod
    public boolean startAutofocus() {
        synchronized (lock) {
            try {
                if (isAutofocusProcessing()) {
                    return true;
                }

                CameraDeviceProtocol_FOCUSMODE = CameraDeviceProtocol.FOCUSMODE_PROCESSING;
                camera.autoFocus(autoFocusCallback);
                return true;
            } catch (Exception e) {
                AndroidUtil.log(e);
            }
            return false;
        }
    }

    /**
     * 現在のフォーカス状態を取り出す
     * 処理中でなければ、NONEに戻される
     * @return
     */
    @JCMethod
    public int popFocusMode() {
        synchronized (lock) {
            int result = CameraDeviceProtocol_FOCUSMODE;
            if (!isAutofocusProcessing()) {
                CameraDeviceProtocol_FOCUSMODE = CameraDeviceProtocol.FOCUSMODE_NONE;
            }

            return result;
        }
    }

    /**
     * オートフォーカス処理をキャンセルする
     * @return
     */
    @JCMethod
    public boolean cancelAutofocus() {
        synchronized (lock) {
            if (!isAutofocusProcessing()) {
                return true;
            }

            try {
                CameraDeviceProtocol_FOCUSMODE = CameraDeviceProtocol.FOCUSMODE_NONE;
                camera.cancelAutoFocus();

                return true;
            } catch (Exception e) {
                AndroidUtil.log(e);
            }
            return false;
        }
    }

    /**
     * プレビュー幅
     * @return
     */
    @JCMethod
    public int getPreviewWidth() {
        return cameraParams.getPreviewSize().width;
    }

    /**
     * プレビュー高さ
     * @return
     */
    @JCMethod
    public int getPreviewHeight() {
        return cameraParams.getPreviewSize().height;
    }

    /**
     * 指定したアスペクト比に近いプレビューサイズを選択する
     * @param width
     * @param height
     * @param minWidth
     * @param minHeight
     */
    @JCMethod
    public void requestPreviewSize(int width, int height, int minWidth, int minHeight, boolean fripWH) {
        final float TARGET_ASPECT = (float) width / (float) height;

        try {
            List<Size> previewSizes = cameraParams.getSupportedPreviewSizes();
            Size target = null;
            float current_diff = 999999999;

            for (Size size : previewSizes) {
                // 最低限のフォーマットは保つ
                if (size.width >= minWidth && size.height >= minHeight) {
                    float aspect_diff = ((float) size.width / (float) size.height) - TARGET_ASPECT;

                    // アスペクト比の差分が小さい＝近い構成をコピーする
                    if (Math.abs(aspect_diff) < current_diff) {
                        target = size;
                        current_diff = aspect_diff;
                    }
                }
            }

            if (target != null) {
                if (fripWH) {
                    AndroidUtil.log(String.format("change preview size(%d x %d)", target.height, target.width));
                    cameraParams.setPreviewSize(target.height, target.width);
                } else {
                    AndroidUtil.log(String.format("change preview size(%d x %d)", target.width, target.height));
                    cameraParams.setPreviewSize(target.width, target.height);
                }
                camera.setParameters(cameraParams);
            }
        } catch (Exception e) {
            AndroidUtil.log(e);
        }

    }

    /**
     * テクスチャに対してプレビューを開始する
     * 
     * DeviceLock済みの状態で呼び出す必要がある
     * @param textureName
     * @return
     */
    @JCMethod
    public boolean startPreview(int textureName) {
        try {
            previewSurface = new PreviewSurface(textureName);
            camera.setPreviewTexture(previewSurface);
            camera.startPreview();
            return true;
        } catch (Exception e) {
            AndroidUtil.log(e);
            if (previewSurface != null) {
                previewSurface.release();
                previewSurface = null;
            }
        }
        return false;
    }

    /**
     * テクスチャのプレビューを停止する
     */
    @JCMethod
    public boolean stopPreview() {
        try {
            if (previewSurface == null) {
                return true;
            }

            textureMatrix = null;
            camera.stopPreview();
            previewSurface.release();
            previewSurface = null;
            return true;
        } catch (Exception e) {
            AndroidUtil.log(e);
        }
        return false;
    }

    /**
     * 管理している資源を解放する
     */
    @JCMethod
    public void dispose() {
        if (previewSurface != null) {
            stopPreview();
        }
        camera.release();
        camera = null;
    }

    /**
     * テクスチャレンダリング用の行列を取得する
     * @return
     */
    @JCMethod
    public FloatBuffer getTextureMatrix() {
        if (previewSurface == null) {
            return null;
        }

        if (textureMatrix == null) {
            textureMatrix = ByteBuffer.allocateDirect(16 * 4).order(ByteOrder.nativeOrder()).asFloatBuffer();
        }
        float[] temp = new float[4 * 4];

        previewSurface.getTransformMatrix(temp);

        boolean ok = false;
        for (float f : temp) {
            if (f != 0) {
                ok = true;
            }
        }
        // 0行列であれば0を返す
        if (!ok) {
            return null;
        }

        textureMatrix.put(temp);
        textureMatrix.position(0);

        return textureMatrix;
    }

    /**
     * カメラのプレビュー情報をテクスチャに焼きこむ
     * @return
     */
    @JCMethod
    public boolean renderingToTexture() {
        if (previewSurface != null) {
            return previewSurface.renderingToTexture();
        } else {
            return false;
        }
    }

    /**
     * インスタンスを作成する
     * @return
     */
    @JCMethod
    public static CameraWrapper createInstance(int CameraDeviceProtocol_TYPE) {
        try {
            CameraWrapper result = new CameraWrapper(CameraDeviceProtocol_TYPE);
            if (result.open()) {
                return result;
            }
        } catch (Exception e) {
            AndroidUtil.log(e);
        }
        return null;
    }

    /**
     * プレビュー用のサーフェイスを示す
     */
    private class PreviewSurface extends SurfaceTexture implements OnFrameAvailableListener {
        /**
         * キャプチャの準備が出来たらtrue
         */
        boolean captured = false;

        PreviewSurface(int textureName) {
            super(textureName);
            setOnFrameAvailableListener(this);
        }

        /**
         * キャプチャコールバックを受け取る
         */
        @Override
        public void onFrameAvailable(SurfaceTexture surfaceTexture) {
            synchronized (this) {
                captured = true;
            }
        }

        /**
         * テクスチャに対して画像を焼きこむ
         * @return
         */
        public boolean renderingToTexture() {
            synchronized (this) {
                // キャプチャ準備ができていたら焼きこむ
                if (captured) {
                    updateTexImage();
                    captured = false;
                    return true;
                }
            }

            return false;
        }
    }

    /**
     * オートフォーカス用
     */
    private final Camera.AutoFocusCallback autoFocusCallback = new AutoFocusCallback() {
        @Override
        public void onAutoFocus(boolean success, Camera camera) {
            synchronized (lock) {
                // キャンセルされていたら何も行わない
                if (!isAutofocusProcessing()) {
                    return;
                }

                if (success) {
                    CameraDeviceProtocol_FOCUSMODE = CameraDeviceProtocol.FOCUSMODE_COMPLETED;
                } else {
                    CameraDeviceProtocol_FOCUSMODE = CameraDeviceProtocol.FOCUSMODE_FAILED;
                }
            }
        }
    };
}
