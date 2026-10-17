#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "keyboardhook.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_checkPreview_checkStateChanged(const Qt::CheckState &);

    void on_checkVertical_checkStateChanged(const Qt::CheckState &);

    void on_spinProgessBar_valueChanged(int);

    void on_spinSize_valueChanged(int);

    void on_spinWarning_valueChanged(int);

    void on_checkBackspace_checkStateChanged(const Qt::CheckState &);

    void on_spinSpace_valueChanged(int);

private:
    Ui::MainWindow *ui;
    KeyboardHook keyboardHook;

    int keyPressed(int code);
};
#endif // MAINWINDOW_H
