/*
 * ArchiveFigureDataFactory.cpp
 *
 *  Created on: 2013/01/17
 */

#include    "jc/graphics/figure/data/ArchiveFigureDataFactory.h"
#include    "jc/platform/Platform.h"
#include    "jc/io/ByteArrayInputStream.h"

#ifdef  BUILD_Android

namespace jc {

ArchiveFigureDataFactory::ArchiveFigureDataFactory() {
    raw_archive.length = 0;
}

ArchiveFigureDataFactory::~ArchiveFigureDataFactory() {

}

/**
 * 初期化を行う
 */
void ArchiveFigureDataFactory::initialize(const Uri &archive_uri) {
    this->archives.reset(mark_new FileArchiveImporter());

    {
        MInputStream stream = Platform::getFileSystem()->openInputStream(archive_uri);
        raw_archive.buffer = InputStream::toByteArray(stream, &raw_archive.length);
    }
    archives->initialize(MInputStream(mark_new ByteArrayInputStream(raw_archive.buffer, raw_archive.length)));

    jclogf("archive files(%d)", archives->getFileCount());
}

/**
 * 入力ストリームを作成する
 */
MBinaryInputStream ArchiveFigureDataFactory::makeStream(const String &name) {
    ArchiveInfo info;
    if (!archives->findFile(name, &info)) {
        throw create_exception(IOException, String::format("File Not Found (%s)", name.c_str()).c_str());
    }

//    jclogf("archive load(%s) %d -> %d", info.file_name, info.file_head, info.file_length);

    MInputStream byteArrayInputStream(mark_new ByteArrayInputStream(raw_archive.buffer, info.file_head, info.file_length));

    return MBinaryInputStream(mark_new BinaryInputStream(byteArrayInputStream));
}

/**
 * 大本のインフォメーションを取得する
 */
MBinaryInputStream ArchiveFigureDataFactory::openFigureInfo() {
    return makeStream("figure.info");
}

/**
 * ノードを読み込む
 */
MBinaryInputStream ArchiveFigureDataFactory::openNode(const s32 nodeNumber) {
    return makeStream(String::format("node%03d.node", nodeNumber));
}

/**
 * 指定したノードのメッシュ情報を読み込む
 */
MBinaryInputStream ArchiveFigureDataFactory::openMeshInfo(const s32 nodeNumber) {
    return makeStream(String::format("node%03d.meshinfo", nodeNumber));
}

/*
 * 指定したマテリアル情報を読み込む
 */
MBinaryInputStream ArchiveFigureDataFactory::openMaterialInfo(const s32 nodeNumber, const s32 materialNumber) {
    return makeStream(String::format("node%03d_mtl%02d.material", nodeNumber, materialNumber));
}

/**
 * メッシュデータを読み込む。
 *
 * 各ノードはマテリアルごと、複数のコンテキストにわかれている。
 * 複数のコンテキストを組み上げることで１つのマテリアル分の頂点データが出来上がる。
 * １メッシュで大量のボーンを使った場合、ベクタユニット数が足りなくなることを防ぐため。
 *
 * @param type 得たい情報のタイプ（拡張子に対応する）
 * @param nodeNumber ノード番号
 * @param materialNumber マテリアル番号
 * @param contextNumber コンテキスト番号
 */
MBinaryInputStream ArchiveFigureDataFactory::openMeshData(const MeshDataType_e type, const s32 nodeNumber, const u32 materialNumber, const u32 contextNumber) {
    const char *ext_table[] = { "indices", "vertices", "coords", "normals", "bones", "weightindices", "weight" };

    return makeStream(String::format("node%03d_mtl%02d_ctx%02d.%s", nodeNumber, materialNumber, contextNumber, ext_table[type]));
}

/**
 * アニメーション用データを開く
 */
MBinaryInputStream ArchiveFigureDataFactory::openAnimationData(const AnimationDataType_e type, const u32 clipNumber, const u32 nodeNumber) {
    const char *ext_table[] = { "t", "r", "s" };
    return makeStream(String::format("node%03d.anim_%s", nodeNumber, ext_table[type]));
}
}

#endif
