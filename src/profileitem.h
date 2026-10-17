#ifndef PROFILEITEM_H
#define PROFILEITEM_H

#include <QDateTime>
#include <QPixmap>

struct ProfileItem {
    void refresh() {
        expiry_at = QDateTime::currentMSecsSinceEpoch() + time_to_live;
    }

    QString name;
    QPixmap const *image;
    double time_to_live{0};
    double expiry_at{0};
    int hotkey{0};
};

#endif // PROFILEITEM_H
