package com.eaglesakura.lib.jc.imp;

import com.eaglesakura.lib.jc.JCUtil;

public abstract class ConverterBase {
    protected static final String indent = JCUtil.INDENT_SPACE;

    protected static final String newline = JCUtil.NEWLINE;

    /**
     * 特定のアノテーションが付与されている場合はtrue
     * @param anno
     * @return
     */
    public abstract boolean hasAnnotation(Class<?> anno);
}
