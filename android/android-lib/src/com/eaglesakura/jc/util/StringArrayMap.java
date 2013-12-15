package com.eaglesakura.jc.util;

import java.util.ArrayList;
import java.util.List;

/**
 * String型のKey-Valueを構築するMap
 * SDKとNDKの橋渡しをサポートする
 */
public class StringArrayMap {

    /**
     * データ配列。長さは必ず偶数になる。
     * datas[n*2 + 0] = key
     * datas[n*2 + 1] = value
     */
    List<String> datas = new ArrayList<String>();

    public StringArrayMap() {
    }

    /**
     * データを構築する
     * @param array
     */
    public StringArrayMap(String[] array) {
        fromArray(array);
    }

    /**
     * データ格納用の配列Indexを取得する。
     * keyが見つからなかったら自動で拡張する
     * @param key
     * @return
     */
    private int getDataIndex(String key) {
        for (int i = 0; i < datas.size(); i += 2) {
            if (datas.get(i).equals(key)) {
                return i + 1;
            }
        }

        datas.add(key);
        datas.add("");

        return datas.size() - 1;
    }

    /**
     * key-valueを追加する
     * @param key
     * @param value
     */
    public void put(String key, Object value) {
        datas.set(getDataIndex(key), value.toString());
    }

    /**
     * key-valueを追加する
     * @param key
     * @param value
     */
    public void put(long key, String value) {
        put(String.valueOf(key), value);
    }

    /**
     * バンクを経由してデータを追加する
     * @param key
     * @param value
     * @param bank
     */
    public void put(String key, Object value, DataBank bank) {
        final long bankKey = bank.add(value);
        put(key, String.valueOf(bankKey));
    }

    /**
     * 値を取得する
     * @param key
     * @return
     */
    public String get(String key) {
        return datas.get(getDataIndex(key));
    }

    /**
     * 整数値を取得する
     * @param key
     * @param def
     * @return
     */
    public int getInteger(String key, int def) {
        try {
            return Integer.parseInt(get(key));
        } catch (Exception e) {
            return def;
        }
    }

    /**
     * 整数値を取得する
     * @param key
     * @param def
     * @return
     */
    public long getLong(String key, long def) {
        try {
            return Long.parseLong(get(key));
        } catch (Exception e) {
            return def;
        }
    }

    /**
     * 浮動小数を取得する
     * @param key
     * @param def
     * @return
     */
    public float getFloat(String key, float def) {
        try {
            return Float.parseFloat(get(key));
        } catch (Exception e) {
            return def;
        }
    }

    /**
     * 実数値を取得する
     * @param key
     * @param def
     * @return
     */
    public double getDouble(String key, double def) {
        try {
            return Double.parseDouble(get(key));
        } catch (Exception e) {
            return def;
        }
    }

    public boolean getBoolean(String key, boolean def) {
        try {
            return Boolean.parseBoolean(get(key));
        } catch (Exception e) {
            return def;
        }
    }

    /**
     * データバンクから値を取得する
     * @param key
     * @param bank
     * @return
     */
    public Object popBank(String key, DataBank bank) {
        try {
            final long bankKey = getLong(key, 0);
            if (bankKey != 0) {
                return bank.pop(bankKey);
            }
        } catch (Exception e) {
        }
        return null;
    }

    /**
     * String配列へ書き出す
     * @return
     */
    public String[] toArray() {
        return toArray(new String[datas.size()]);
    }

    /**
     * String配列へ書き出す
     * @param write
     * @return
     */
    public String[] toArray(String[] write) {
        return datas.toArray(write);
    }

    /**
     * String配列から書き戻す
     * @param keyValues
     */
    public void fromArray(String[] keyValues) {
        datas.clear();
        for (String s : keyValues) {
            datas.add(s);
        }
    }
}
