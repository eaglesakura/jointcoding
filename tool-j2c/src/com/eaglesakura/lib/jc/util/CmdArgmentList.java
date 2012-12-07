package com.eaglesakura.lib.jc.util;

import java.util.ArrayList;
import java.util.List;

/**
 * 入力されたコマンド一覧を管理する
 * 
 *
 */
public class CmdArgmentList {
    List<CmdArgment> argments = null;

    public CmdArgmentList(String[] args) {
        argments = CmdArgment.parse(args);
    }

    /**
     * 特定の引数を取得する
     * @param command
     * @return
     */
    public CmdArgment getArgmentParams(String command) {
        for (CmdArgment arg : argments) {
            if (arg.getCommand().equals(command)) {
                return arg;
            }
        }
        return null;
    }

    /**
     * 引数の第1パラメーターを取得する
     * @param command
     * @return
     */
    public String getArgmentParam(String command) {
        CmdArgment arg = getArgmentParams(command);
        if (arg == null) {
            return null;
        }
        List<String> argments2 = arg.getArgments();

        if (argments2.isEmpty()) {
            return null;
        }
        return argments2.get(0);
    }

    /**
     * 指定したコマンドを持っていたらtrue
     * @param command
     * @return
     */
    public boolean hasArgment(String command) {
        return getArgmentParams(command) != null;
    }

    /**
     * 指定したコマンドに一致するものを列挙する
     * @param command
     * @return
     */
    public List<CmdArgment> getArgments(String command) {
        List<CmdArgment> result = new ArrayList<CmdArgment>();
        for (CmdArgment arg : argments) {
            if (arg.getCommand().equals(command)) {
                result.add(arg);
            }
        }
        return result;
    }
}
