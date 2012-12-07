package com.eaglesakura.lib.jc;

import java.io.StringWriter;

/**
 * ソースコードを出力するための専用ライター
 * 
 *
 */
public class SourceCodeWriter extends StringWriter {
    /**
     * 現在の出力タブの階層
     */
    int tabDepth = 0;

    /**
     * 改行を行う
     */
    public void newline() {
        append(JCUtil.NEWLINE);

        // 現在の階層に合わせてインデントを行う
        for (int i = 0; i < tabDepth; ++i) {
            append(JCUtil.INDENT_SPACE);
        }
    }

    /**
     * 改行を行う
     * @param tab
     */
    public void newline(boolean tab) {
        append(JCUtil.NEWLINE);

        if (tab) {
            // 現在の階層に合わせてインデントを行う
            for (int i = 0; i < tabDepth; ++i) {
                indent();
            }
        }
    }

    public SourceCodeWriter add(char c) {
        append(c);
        return this;
    }

    public SourceCodeWriter add(CharSequence c) {
        append(c);
        return this;
    }

    /**
     * インデントを加える
     */
    public SourceCodeWriter indent() {
        append(JCUtil.INDENT_SPACE);
        return this;
    }

    /**
     * タブを1階層増やす
     */
    public void pushTab() {
        ++tabDepth;
    }

    /**
     * タブを1階層減らす
     */
    public void popTab() {
        if (tabDepth > 0) {
            --tabDepth;
        }
    }
}
