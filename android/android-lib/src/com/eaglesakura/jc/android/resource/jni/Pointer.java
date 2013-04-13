package com.eaglesakura.jc.android.resource.jni;

import com.eaglesakura.jc.android.resource.DisposableResource;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * ポインタを保持する。
 * release()結果として参照が0以下になった場合、deleteが呼び出される。
 * ただし、デストラクタの起動については保証されない。
 * 
 *
 */
@JCClass(
         cppNamespace = "ndk")
public class Pointer extends DisposableResource {
    public static final int NULL = 0x00000000;

    /**
     * ポインタモード
     * 
     *
     */
    public enum Mode {
        /**
         * なんでも扱うポインタ
         */
        VoidPointer {
            @Override
            public void delete(int pointer) {
                deleteVoidPtr(pointer);
            }

            /**
             * ナマのポインタを取得する
             * @param pointer
             * @return
             */
            public int rawPointer(int pointer) {
                return pointer;
            }
        },

        /**
         * Objectを継承したポインタ
         */
        Object {
            @Override
            public void delete(int pointer) {
                deleteObjectPtr(pointer);
            }

            /**
             * ナマのポインタを取得する
             * @param pointer
             * @return
             */
            public int rawPointer(int pointer) {
                return pointer;
            }
        },

        /**
         * jc_sp<Object>で管理するポインタ
         */
        SharedObject {
            @Override
            public void delete(int pointer) {
                deleteSharedObjectPtr(pointer);
            }

            @Override
            public int rawPointer(int pointer) {
                return getSharedObjectPtr(pointer);
            }
        };

        /**
         * ポインタの削除を行う
         * @param pointer
         */
        public abstract void delete(int pointer);

        /**
         * 実際に管理したいオブジェクトを取得する。
         * 例えば、jc_sp<>であれば実際に管理しているオブジェクトを返して欲しい。
         * @param pointer
         * @return
         */
        public abstract int rawPointer(int pointer);
    }

    /**
     * ポインタモード
     */
    private Mode mode = Mode.VoidPointer;

    /**
     * 保持しているポインタ。
     */
    private int pointer = NULL;

    public Pointer(int pointer, Mode mode) {
        this.pointer = pointer;
        this.mode = mode;
    }

    public Pointer(Mode mode) {
        this.pointer = NULL;
        this.mode = mode;
    }

    /**
     * 保持しているリソースを解放する
     */
    @JCMethod
    @Override
    public void dispose() {
        if (pointer != NULL) {
            mode.delete(pointer);
        }
        pointer = 0x00;
    }

    /**
     * 実際のポインタ値を取得する
     * @return
     */
    public int getNativePointer() {
        return pointer;
    }

    /**
     * ナマのポインタを取得する。
     * @return
     */
    @JCMethod
    public int getObjectPointer() {
        if (pointer == NULL) {
            return NULL;
        }
        return mode.rawPointer(pointer);
    }

    /**
     * ポインタの制御モードを取得する
     * @return
     */
    public Mode getMode() {
        return mode;
    }

    /**
     * 
     * @param pointer
     */
    @JCMethod(
              nativeMethod = true)
    static native void deleteVoidPtr(int pointer);

    @JCMethod(
              nativeMethod = true)
    static native void deleteObjectPtr(int pointer);

    @JCMethod(
              nativeMethod = true)
    static native void deleteSharedObjectPtr(int pointer);

    /**
     * jc_sp<Object>* のポインタから、実際のオブジェクトのポインタを取得する
     * @param jc_sp_pointer
     * @return
     */
    @JCMethod(
              nativeMethod = true)
    static native int getSharedObjectPtr(int jc_sp_pointer);
}
