#ifndef PROFILE_H
#define PROFILE_H

#include "profileitem.h"

#include <QVector>

struct Profile {
    QString name;
    QVector<ProfileItem> items;

    void refresh(int code) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->hotkey == code) {
                it->refresh();
                return;
            }
        }
    }
};

#endif // PROFILE_H
