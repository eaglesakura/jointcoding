package com.eaglesakura.lib.jc.codetemp;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * ソースコードのテンプレートを構築する。
 * 
 *
 */
public class CodeTemplate {

    /**
     * 元々のテンプレート
     */
    String templateText = null;

    /**
     * 書き出し用のコード
     */
    String code = null;

    public CodeTemplate(String fileName) {
        try {
            StringBuffer codeBuffer = new StringBuffer();
            //            InputStream is = new FileInputStream(templateFile);
            InputStream is = getClass().getResourceAsStream(fileName);
            {
                String line = null;
                BufferedReader reader = new BufferedReader(new InputStreamReader(is));
                while ((line = reader.readLine()) != null) {
                    codeBuffer.append(line).append("\n");
                }
            }
            is.close();

            this.templateText = codeBuffer.toString();
            this.code = templateText;
        } catch (Exception e) {

        }
    }

    /**
     * コードのメタ文字を実際のコードに変換する
     * @param origin
     * @param rep
     */
    public void replase(String origin, String rep) {
        try {
            code = code.replace(origin, rep);
        } catch (RuntimeException e) {
            try {
                code = code.replace(origin, rep);
            } catch (RuntimeException ee) {
                System.out.println(String.format("rep = " + rep));
                System.out.println(String.format("origin = " + origin));
                throw ee;
            }
            //            throw e;
        }
    }

    /**
     * コード本体を取得する
     * @return
     */
    public String getCode() {
        return code;
    }
}
