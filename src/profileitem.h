#ifndef PROFILEITEM_H
#define PROFILEITEM_H

#include <QPixmap>
#include <chrono>

namespace Chrono = std::chrono;
using Clock = Chrono::steady_clock;

struct ProfileItem {
    void refresh() {
        expiry_at = Clock::now() + time_to_live;
    }

    QString name;
    QPixmap const *image;
    Clock::duration time_to_live{0};
    Clock::time_point expiry_at{};
    int hotkey{0};
};

#endif // PROFILEITEM_H
