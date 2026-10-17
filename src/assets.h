#ifndef ASSETS_H
#define ASSETS_H

#include <QPixmap>
#include <QString>
#include <QVector>

struct AssetImage {
    explicit AssetImage(const QString &file_name, const QString &path) : name(file_name), image(path) {
    }
    QString name;
    QPixmap image;
};

class Assets {
public:
    static Assets &the();
    void init(const QString &path);
    const QVector<AssetImage> &images();
    const QPixmap *getImageByName(const QString &name);

private:
    Assets();
    QVector<AssetImage> m_images;
};

#endif // ASSETS_H
