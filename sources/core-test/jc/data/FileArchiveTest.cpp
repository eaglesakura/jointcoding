/*
 * FileArchiveTest.cpp
 *
 *  Created on: 2013/01/17
 */

#include    "jointcoding.h"
#include    "jointcoding-testsupport.h"
#include    "jc/data/FileArchiveImporter.h"
#include    "jc/data/FileArchiveOutputStream.h"
#include    "jc/io/FileOutputStream.h"
#include    "jc/io/FileInputStream.h"
#include    "jc/io/File.h"

#include    <dirent.h>

using namespace jc;

static std::vector<File> listFiles() {
    std::vector<File> result;

    DIR *dir = opendir("./");

    dirent *dp;

    while ((dp = readdir(dir)) != NULL) {

        Uri uri = Uri::fromLocalStrage(dp->d_name);
        if (dp->d_type != DT_DIR) {
            File file = File::fromFile(uri);

            const String ext = file.getExt();
            if (
            //anim
            ext == "anim_r" || ext == "anim_t" || ext == "anim_s"
            // figure
                    || ext == "info" || ext == "node" || ext == "meshinfo" || ext == "bones" || ext == "vertices" || ext == "coords" || ext == "indices" || ext == "normals" || ext == "weight" || ext == "weightindices" || ext == "material") {
//                jclogf("path = %s", dp->d_name);
                result.push_back(file);
            }
        }
    }
    closedir(dir);

    return result;
}

TESTFUNCTION void test_makeFileArchive() {
    std::vector<File> src_files = listFiles();

    jcboolean completed;
    {
        MOutputStream stream(new FileOutputStream("figure.archive", &completed));
        _assertTrue(completed);

        MFileArchiveOutputStream archive(new FileArchiveOutputStream(stream));

        for (u32 i = 0; i < src_files.size(); ++i) {
            File file = src_files[i];

            MInputStream input(new FileInputStream(file.getName(), &completed));
            _assertTrue(completed);

            u32 file_length;
            jc_sa<u8> file_array = InputStream::toByteArray(input, &file_length);

            archive->beginFile(file.getName());
            archive->write(file_array.get(), file_length);
            archive->endFile();

            jclogf(" file[%s] size(%d)", file.getName().c_str(), file_length);
        }

        archive->close();
    }

    {
        MInputStream stream(new FileInputStream("figure.archive", &completed));
        _assertTrue(completed);

        MFileArchiveImporter importer(new FileArchiveImporter());
        importer->initialize(stream);

        jclogf( "archive files[%d]", importer->getFileCount());
        jclogf( "enum files[%d]", src_files.size());
        _assertEquals(src_files.size(), importer->getFileCount());

    }
}
