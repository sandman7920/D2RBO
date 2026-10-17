#include "theme.h"
#include <QApplication>
#include <QSettings>
#include <QStyleFactory>
#include <QStyleHints>

namespace {
QStringList themes;

void init() {
    if (QSysInfo::productType() == "windows" && QSysInfo::productVersion() == "11") {
        themes.emplace_back("Windows 11");
    }
    themes.emplace_back("Windows 10");
    themes.emplace_back("Fusion");
    themes.emplace_back("Fusion Dark");
    themes.emplace_back("Windows Classic");
}

} // namespace

const QStringList &Theme::list() {
    return themes;
}

void Theme::load() {
    if (themes.isEmpty()) {
        init();
    }
    auto theme = QSettings().value("theme", "").toString();
    if (theme == "") {
        theme = themes.front();
        QSettings().setValue("theme", theme);
    }

    if (theme == "Windows 10") {
        QApplication::setStyle(QStyleFactory::create("windowsvista"));
        return;
    }

    if (theme == "Windows 11") {
        QApplication::setStyle(QStyleFactory::create("windows11"));
        return;
    }

    if (theme == "Fusion") {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        return;
    }

    if (theme == "Fusion Dark") {
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        QGuiApplication::styleHints()->setColorScheme(Qt::ColorScheme::Dark);
        return;
    }

    if (theme == "Windows Classic") {
        QApplication::setStyle(QStyleFactory::create("Windows"));
        return;
    }
}

void Theme::save(QString theme) {
    QSettings().setValue("theme", theme);
}

Theme::Theme() = default;
