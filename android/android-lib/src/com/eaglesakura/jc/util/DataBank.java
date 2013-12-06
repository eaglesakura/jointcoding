package com.eaglesakura.jc.util;

import java.util.HashMap;
import java.util.Map;

import com.eaglesakura.lib.jc.annotation.jnimake.JCClass;
import com.eaglesakura.lib.jc.annotation.jnimake.JCMethod;

/**
 * 任意のオブジェクトをバンクとして預けておく事ができる。
 * pullしない限りメモリを食い続けるので、利用する場合は注意が必要
 * 
 * 0キーは常に無効
 */
@JCClass(
         cppNamespace = "ndk")
public class DataBank {

    /**
     * オブジェクト本体
     */
    private Map<Long, Object> values = new HashMap<Long, Object>();

    private Object lock = new Object();

    private long keySeed = 1;

    public DataBank() {
    }

    /**
     * 一意の管理キーを生成する
     * @return
     */
    private long newKey() {
        synchronized (lock) {
            return keySeed++;
        }
    }

    /**
     * バンクへアイテムを追加する。
     * アイテムを取得する際は発行されたKeyを利用する。
     * @param obj
     * @return
     */
    @JCMethod
    public long add(Object obj) {
        synchronized (lock) {
            long key = newKey();
            values.put(key, obj);
            return key;
        }
    }

    /**
     * 預けていたオブジェクトを取り出す。
     * オブジェクトが見つからなかったらnullを返す。
     * @param key
     * @return
     */
    @JCMethod
    public Object pop(long key) {
        synchronized (lock) {
            Object result = values.get(key);
            if (result != null) {
                values.remove(result);
            }
            return result;
        }
    }

    /**
     * 文字列を取得する
     * @param key
     * @return
     */
    @JCMethod
    public String popString(long key) {
        return (String) pop(key);
    }

    /**
     * 整数を取得する
     * @param key
     * @return
     */
    @JCMethod
    public int popInt(long key) {
        try {
            Object obj = pop(key);
            if (obj != null) {
                return (Integer) obj;
            } else {
                return 0;
            }
        } catch (Exception e) {
            return 0;
        }
    }

    /**
     * 整数を取得する
     * @param key
     * @return
     */
    @JCMethod
    public long popLong(long key) {
        try {
            Object obj = pop(key);
            if (obj != null) {
                return (Long) obj;
            } else {
                return 0;
            }
        } catch (Exception e) {
            return 0;
        }
    }

    /**
     * 実数を取得する
     * @param key
     * @return
     */
    @JCMethod
    public float popFloat(long key) {
        try {
            Object obj = pop(key);
            if (obj != null) {
                return (Float) obj;
            } else {
                return 0;
            }
        } catch (Exception e) {
            return 0;
        }
    }

    /**
     * 実数を取得する
     * @param key
     * @return
     */
    @JCMethod
    public double popDouble(long key) {
        try {
            Object obj = pop(key);
            if (obj != null) {
                return (Double) obj;
            } else {
                return 0;
            }
        } catch (Exception e) {
            return 0;
        }
    }

}
