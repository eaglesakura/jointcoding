package com.eaglesakura.lib.jc.type;

public enum JniMethodResultType {

    /**
     * 戻り値無し
     */
    Void,

    /**
     * 
     */
    Byte,

    /**
     * 
     */
    Char,

    /**
     * 
     */
    Short,

    /**
     * 
     */
    Int,

    /**
     * 
     */
    Long,

    /**
     * 
     */
    Float,

    /**
     * 
     */
    Double,

    /**
     * 
     */
    Boolean,

    /**
     * 
     */
    Object, ;

    /**
     * 呼び出しメソッド名を取得する
     * @return
     */
    public String getCallMethodName(boolean isStatic) {
        if (isStatic) {
            return "CallStatic" + name() + "Method";
        } else {
            return "Call" + name() + "Method";
        }
    }
}
