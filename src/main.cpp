#include "assets.h"
#include "mainwindow.h"
#include "overlay.h"
#include "theme.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[]) {
    QGuiApplication::setDesktopFileName("d2rbo");
    QCoreApplication::setOrganizationName("d2rbo");
    QCoreApplication::setApplicationName("D2R BO");
    QApplication a(argc, argv);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, QApplication::applicationDirPath());

    Theme::load();

    QString assets_path;
#ifdef ASSETS_PATH
    assets_path = ASSETS_PATH;
#else
    assets_path = QCoreApplication::applicationDirPath();
#endif

    Assets::the().init(assets_path);
    Overlay::the().showFullScreen();

    MainWindow w;
    w.show();
    return a.exec();
}
