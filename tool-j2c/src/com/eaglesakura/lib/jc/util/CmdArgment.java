package com.eaglesakura.lib.jc.util;

import java.util.ArrayList;
import java.util.List;

/**
 * コマンドラインで与えられる引数リストを取得する。
 * コマンドは引数あり・無しに対応する
 * --コマンド
 * --コマンド arg0 arg1
 * 
 *
 */
public class CmdArgment {
    /**
     * コマンド名
     */
    String command = null;

    /**
     * 引数リスト
     */
    List<String> argments = new ArrayList<String>();

    private CmdArgment() {

    }

    public String getCommand() {
        return command;
    }

    /**
     * 引数リストを取得する
     * @return
     */
    public List<String> getArgments() {
        return new ArrayList<String>(argments);
    }

    /**
     * 引数リストをパースする
     * @param args
     * @return
     */
    public static List<CmdArgment> parse(String[] args) {
        List<CmdArgment> result = new ArrayList<CmdArgment>();

        try {
            CmdArgment current = null;
            for (String word : args) {
                if (word.startsWith("--")) {
                    if (current != null) {
                        result.add(current);
                        current = null;
                    }
                    // コマンドを指定している
                    current = new CmdArgment();
                    current.command = word.substring(2);
                } else {
                    if (current != null) {
                        current.argments.add(word);
                    }
                }
            }

            if (current != null) {
                result.add(current);
                current = null;
            }
        } catch (Exception e) {

        }

        return result;
    }
}
