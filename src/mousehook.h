#ifndef MOUSEHOOK_H
#define MOUSEHOOK_H

#include <QObject>
#include <QPoint>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class MouseHook : public QObject {
    Q_OBJECT
public:
    explicit MouseHook(QObject *parent = nullptr);
    ~MouseHook();

    bool installHook();
    void removeHook();

signals:
    int mousePressed(Qt::MouseButton button, QPoint pos);
    int mouseReleased(Qt::MouseButton button, QPoint pos);
    int mouseMove(QPoint pos);

private:
#ifdef Q_OS_WIN
    static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hHook;
    static MouseHook *instance;
#endif
};

#endif // MOUSEHOOK_H