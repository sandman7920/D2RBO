#ifndef THEME_H
#define THEME_H

#include <QStringList>

class Theme {
public:
    static const QStringList &list();
    static void load();
    static void save(QString theme);

private:
    Theme();
};

#endif // THEME_H
