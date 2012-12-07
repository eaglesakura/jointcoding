package com.eaglesakura.lib.jc.imp;

import java.io.File;

import org.junit.Test;

import com.eaglesakura.lib.android.game.util.LogUtil;

public class ClassImporterTest {
    public ClassImporterTest() {
        LogUtil.setOutput(true);
    }

    @Test
    public void fromDirectory() {
        ClassesImporter.fromDirectory(new File("./bin"));
    }

    @Test
    public void fromJar() {
        ClassesImporter.fromJar(new File("/dev-home/android/sdk/platforms/android-14/android.jar"));
    }
}
