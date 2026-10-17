#include "assets.h"
#include <QDir>

Assets::Assets() = default;

Assets &Assets::the() {
    static Assets instance;
    return instance;
}

const QPixmap *Assets::getImageByName(const QString &name) {
    for (auto it = m_images.begin(); it != m_images.end(); ++it) {
        if (it->name == name) {
            return &it->image;
        }
    }
    return nullptr;
}

void Assets::init(const QString &path) {
    QDir img(path + "/images");
    const auto files = img.entryInfoList(QDir::Files, QDir::Name);
    for (auto it = files.begin(); it != files.end(); ++it) {
        auto name = it->fileName();
        const auto pos = name.lastIndexOf('.');
        if (pos != -1) {
            name = name.left(pos);
        }
        m_images.emplace_back(name, it->filePath());
    }
}

const QVector<AssetImage> &Assets::images() {
    return m_images;
}