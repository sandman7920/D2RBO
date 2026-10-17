#include "mainwindow.h"
#include "overlay.h"
#include "theme.h"

#include "ui_mainwindow.h"

#include <QGuiApplication>
#include <QMessageBox>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    for (const auto &t : Theme::list()) {
        ui->comboTheme->addItem(t);
    }
    ui->comboTheme->setCurrentText(QSettings().value("theme").toString());
    connect(ui->comboTheme, &QComboBox::currentTextChanged, this, [this](const QString &value) {
        Theme::save(value);
        QMessageBox::information(this, "Theme Changed",
                                 "In order for changes to take effect, please restart the application.");
    });

    connect(&keyboardHook, &KeyboardHook::keyPressed, this, &MainWindow::keyPressed);
    connect(&Overlay::the(), &Overlay::positionChanged, this,
            [](QPoint p) { QSettings().setValue("overlayPosition", p); });

    connect(ui->profiles, &Profiles::profileChanged, this, [this] {
        if (ui->checkPreview->isChecked()) {
            Overlay::the().setPreview(true);
        }
        Overlay::the().update();
    });

    QSettings settings;
    // set default settings
    ui->checkVertical->setChecked(settings.value("vertical", false).toBool());
    ui->spinProgessBar->setValue(settings.value("progressBar", 3).toInt());
    ui->spinSize->setValue(settings.value("skillSize", 32).toInt());
    ui->spinWarning->setValue(settings.value("skillWarning", 10).toInt());
    ui->checkBackspace->setChecked(settings.value("useBackspace", true).toBool());
    ui->spinSpace->setValue(settings.value("skillSpace", 4).toInt());
    Overlay::the().setPosition(settings.value("overlayPosition", QPoint(80, 140)).toPoint());
}

MainWindow::~MainWindow() {
    delete ui;
};

int MainWindow::keyPressed(int code) {
    if (code == VK_BACK) {
        if (ui->checkBackspace->isChecked()) {
            Overlay::the().toggleVisible();
        }
    } else {
        Overlay::the().refresh(code);
    }
    return 0;
}

void MainWindow::on_btnStart_clicked() {
    ui->checkPreview->setEnabled(false);
    ui->checkPreview->setChecked(false);
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);

    if (!keyboardHook.installHook()) {
        qDebug() << "Global keyboard hook install failed!";
    }

    Overlay::the().start();
    Overlay::the().activateWindow();
}

void MainWindow::on_btnStop_clicked() {
    ui->checkPreview->setEnabled(true);
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);

    keyboardHook.removeHook();
    Overlay::the().stop();
}

void MainWindow::on_checkPreview_checkStateChanged(const Qt::CheckState &state) {
    Overlay::the().setPreview(state == Qt::Checked);
}

void MainWindow::on_checkVertical_checkStateChanged(const Qt::CheckState &state) {
    QSettings().setValue("vertical", state == Qt::Checked);
    Overlay::the().calculateLayout(static_cast<OverlayDirection>(state == Qt::Checked));
}

void MainWindow::on_spinProgessBar_valueChanged(int value) {
    QSettings().setValue("progressBar", value);
    Overlay::the().setProgressBar(value);
}

void MainWindow::on_spinSize_valueChanged(int value) {
    QSettings().setValue("skillSize", value);
    Overlay::the().setSize(value);
}

void MainWindow::on_spinWarning_valueChanged(int value) {
    QSettings().setValue("skillWarning", value);
    Overlay::the().setWarning(value);
}

void MainWindow::on_checkBackspace_checkStateChanged(const Qt::CheckState &state) {
    QSettings().setValue("useBackspace", state == Qt::Checked);
}

void MainWindow::on_spinSpace_valueChanged(int value) {
    QSettings().setValue("skillSpace", value);
    Overlay::the().setSpace(value);
}
