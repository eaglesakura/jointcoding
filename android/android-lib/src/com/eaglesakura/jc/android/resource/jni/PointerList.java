package com.eaglesakura.jc.android.resource.jni;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Map.Entry;

import android.os.Bundle;

import com.eaglesakura.jc.android.IJointCodingClass;
import com.eaglesakura.jc.android.resource.jni.Pointer.Mode;
import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * 複数のポインタをまとめて管理するためのクラス。
 *
 */
@JCClass(
         cppNamespace = "ndk")
public class PointerList implements IJointCodingClass {
    /**
     * 管理しているポインタ一覧
     */
    Map<String, Pointer> pointers = new HashMap<String, Pointer>();

    /**
     * ポインタのキー一覧を保存する
     */
    private static final String BUNDLE_KEY_POINTERKEYS = "POINTER_LIST_BUNDLE_KEY_POINTERKEYS";

    /**
     * ポインタの本体を保存する
     */
    private static final String BUNDLE_KEY_NATIVEPOINTERS = "POINTER_LIST_BUNDLE_KEY_NATIVEPOINTERS";

    /**
     * ポインタのモード一覧を保存する
     */
    private static final String BUNDLE_KEY_POINTERMODES = "BUNDLE_KEY_POINTERMODES";

    public PointerList() {

    }

    /**
     * ポインタの保存を行う
     * @param bundle
     */
    public void save(Bundle bundle) {
        String[] keys = new String[pointers.size()];
        String[] modes = new String[pointers.size()];
        int[] rawPointers = new int[pointers.size()];

        int index = 0;
        Iterator<Entry<String, Pointer>> iterator = pointers.entrySet().iterator();
        while (iterator.hasNext()) {
            Entry<String, Pointer> entry = iterator.next();

            keys[index] = entry.getKey();
            rawPointers[index] = entry.getValue().getNativePointer();
            modes[index] = entry.getValue().getMode().toString();
            ++index;
        }

        bundle.putStringArray(BUNDLE_KEY_POINTERKEYS, keys);
        bundle.putIntArray(BUNDLE_KEY_NATIVEPOINTERS, rawPointers);
        bundle.putStringArray(BUNDLE_KEY_POINTERMODES, modes);
    }

    /**
     * Bundleからの復元を行う
     * @param bundle
     */
    public void restore(Bundle bundle) {
        String[] keys = bundle.getStringArray(BUNDLE_KEY_POINTERKEYS);
        int[] nativePointers = bundle.getIntArray(BUNDLE_KEY_NATIVEPOINTERS);
        String[] modes = bundle.getStringArray(BUNDLE_KEY_POINTERMODES);
        if (keys == null || nativePointers == null) {
            return;
        }
        final int length = keys.length;
        for (int i = 0; i < length; ++i) {
            add(keys[i], nativePointers[i], Pointer.Mode.valueOf(modes[i]));
        }
    }

    /**
     * ポインタ値を取得する。
     * @param key
     * @return
     */
    public int getNativePointer(String key) {
        Pointer p = pointers.get(key);
        if (p != null) {
            return p.getNativePointer();
        } else {
            return 0;
        }
    }

    /**
     * オブジェクト本体のポインタ値を取得する
     * @param key
     * @return
     */
    public int getNativeObjectPointer(String key) {
        Pointer p = pointers.get(key);
        if (p != null) {
            return p.getObjectPointer();
        } else {
            return 0;
        }
    }

    /**
     * 整数をキーにしてポインタを取得する
     * @param key
     * @return
     */
    public int getNativePointer(int key) {
        return getNativePointer(Integer.toString(key));
    }

    /**
     * 整数値をキーにポインタを追加する
     * @param key
     * @param nativePointer
     */
    public void add(int key, int nativePointer, Mode mode) {
        add(Integer.toString(key), nativePointer, mode);
    }

    /**
     * ポインタの追加を行う
     * @param key
     * @param nativePointer
     */
    public void add(String key, int nativePointer, Pointer.Mode mode) {
        pointers.put(key, new Pointer(nativePointer, mode));
    }

    /**
     * 
     * @param key
     * @param pointer
     */
    public void add(String key, Pointer pointer) {
        if (pointer == null) {
            throw new NullPointerException(key + " == null");
        }

        if (pointers.get(key) != null) {
            throw new IllegalArgumentException(key + " is added!!");
        }

        pointers.put(key, pointer);
    }

    /**
     * 整数をキーにして消去する
     * @param key
     */
    @JCMethod
    public void remove(int key) {
        remove(Integer.toString(key));
    }

    /**
     * ポインタの消去を行う
     * @param key
     */
    @JCMethod
    public void remove(String key) {
        Pointer p = pointers.remove(key);
        if (p != null) {
            p.dispose();
        }
    }

    /**
     * 全部のポインタを開放する
     */
    public void clear() {
        Iterator<Entry<String, Pointer>> iterator = pointers.entrySet().iterator();
        while (iterator.hasNext()) {
            Entry<String, Pointer> entry = iterator.next();
            entry.getValue().dispose();
        }
        pointers.clear();
    }
}
