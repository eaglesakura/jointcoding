package com.eaglesakura.lib.jc.util;

import org.junit.Test;

import com.eaglesakura.lib.android.game.util.LogUtil;

public class CmdArgmentListTest {
    public CmdArgmentListTest() {
        LogUtil.setOutput(true);
    }

    @Test
    public void argmentParseTest() {
        String[] argments = {
                "--library", "./bin", "${ANDROID_HOME}/bin", "--output", "./jcgen"
        };

        CmdArgmentList list = new CmdArgmentList(argments);

        LogUtil.log(list.getArgmentParam("library"));
        LogUtil.log(list.getArgmentParam("output"));
    }
}
