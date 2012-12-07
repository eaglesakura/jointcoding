package com.eaglesakura.lib.jc.exp.callsupport;

import java.io.File;

import javassist.CtClass;

import org.junit.Test;

import com.eaglesakura.lib.android.game.util.LogUtil;
import com.eaglesakura.lib.jc.imp.ClassConverter;
import com.eaglesakura.lib.jc.imp.ClassesImporter;
import com.eaglesakura.lib.jc.sample.jnimake.SampleExportClass;
import com.eaglesakura.lib.jc.sample.jnimake.SampleInterface;

public class CppExporterTest {
    public CppExporterTest() {
        LogUtil.setOutput(true);
    }

    @Test
    public void implExportTest() {
        ClassesImporter.fromDirectory(new File("./bin"));

        CtClass clazz = ClassesImporter.get(SampleExportClass.class);
        LogUtil.log("clazz :: " + clazz);

        ClassConverter converter = new ClassConverter(clazz);
        {
            CppHeaderExporter exporter = new CppHeaderExporter(converter);
            LogUtil.log("---------------- export header ----------------");
            LogUtil.log(exporter.export().toString());
        }
        {
            CppImplExporter exporter = new CppImplExporter(converter);
            LogUtil.log("---------------- export impl ----------------");
            LogUtil.log(exporter.export().toString());
        }
    }

    @Test
    public void protocolExportTest() {
        ClassesImporter.fromDirectory(new File("./bin"));
        CtClass clazz = ClassesImporter.get(SampleInterface.class);

        ClassConverter converter = new ClassConverter(clazz);

        {
            CppHeaderExporter exporter = new CppHeaderExporter(converter);
            LogUtil.log("---------------- export header ----------------");
            LogUtil.log(exporter.export().toString());
        }
        {
            CppImplExporter exporter = new CppImplExporter(converter);
            LogUtil.log("---------------- export impl ----------------");
            LogUtil.log(exporter.export().toString());
        }
    }
}
