package com.eaglesakura.lib.jc.type;

public enum JniArgmentType {
    /**
     * 符号付き1バイト整数
     */
    Byte {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Byte;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }

        @Override
        public java.lang.String getFieldType() {
            return "::jc::s8";
        }
    },

    /**
     * バイト配列
     */
    ByteArray {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 文字
     */
    Char {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Char;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 文字配列
     */
    CharArray {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 2バイト整数
     */
    Short {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Short;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }

        @Override
        public java.lang.String getFieldType() {
            return "::jc::s16";
        }
    },

    /**
     * 文字配列
     */
    ShortArray {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 4バイト整数
     */
    Int {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Int;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }

        @Override
        public java.lang.String getFieldType() {
            return "::jc::s32";
        }
    },

    /**
     * 整数配列
     */
    IntArray {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 8バイト整数
     */
    Long {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Long;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }

        @Override
        public java.lang.String getFieldType() {
            return "::jc::s64";
        }
    },

    /**
     * 整数配列
     */
    LongArray {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 浮動小数
     */
    Float {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Float;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }

        @Override
        public java.lang.String getFieldType() {
            return "float";
        }
    },

    /**
     * 浮動小数配列
     */
    FloatArray {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 倍精度実数
     */
    Double {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Double;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }

        @Override
        public java.lang.String getFieldType() {
            return "double";
        }
    },

    /**
     * 実数配列
     */
    DoubleArray {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 真偽
     */
    Boolean {
        @Override
        public java.lang.String getFieldType() {
            return "::jc::jcboolean";
        }

        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Boolean;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }
    },

    BooleanArray {
        @Override
        public java.lang.String getFieldType() {
            return null;
        }

        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

    },

    /**
     * 文字列
     */
    String {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }

        @Override
        public java.lang.String getFieldType() {
            return "::jc::charactor*";
        }
    },

    /**
     * 通常のオブジェクト
     */
    Object {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * オブジェクトの配列
     */
    ObjectArray {
        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Object;
        }

        @Override
        public boolean isFieldSupport() {
            return false;
        }

        @Override
        public java.lang.String getFieldType() {
            return null;
        }
    },

    /**
     * 戻り値なし
     */
    Void {
        @Override
        public java.lang.String getJniName() {
            return "void";
        }

        @Override
        public JniMethodResultType getResultType() {
            return JniMethodResultType.Void;
        }

        @Override
        public boolean isFieldSupport() {
            return true;
        }

        @Override
        public java.lang.String getFieldType() {
            return "void";
        }
    };

    public String getJniName() {
        String result = "j" + (name().toLowerCase());
        if (result.endsWith("array")) {
            result = result.replace("array", "Array");
        }
        return result;
    }

    public abstract JniMethodResultType getResultType();

    /**
     * static値の出力に対応しているならtrue
     * @return
     */
    public abstract boolean isFieldSupport();

    /**
     * static固定値用の型名を取得する
     * @return
     */
    public abstract String getFieldType();

    /**
     * ClazzからJniで使用されるべき型を取得する
     * @param clazz
     * @return
     */
    public static JniArgmentType getType(String name) {
        JniArgmentType[] values = JniArgmentType.values();
        if (name.endsWith("[]")) {
            name = name.substring(0, name.length() - 2);
            name += "Array";
        }
        for (JniArgmentType type : values) {
            if (type.name().toUpperCase().equals(name.toUpperCase())) {
                return type;
            }
        }

        if (name.endsWith("Array")) {
            return ObjectArray;
        } else {
            return Object;
        }
    }
}
