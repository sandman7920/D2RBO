#ifndef KEYBOARDHOOK_H
#define KEYBOARDHOOK_H

#include <QObject>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class KeyboardHook : public QObject {
    Q_OBJECT
public:
    explicit KeyboardHook(QObject *parent = nullptr);
    ~KeyboardHook();

    bool installHook();
    void removeHook();

signals:
    void keyPressed(int vkCode);
    void keyReleased(int vkCode);

private:
#ifdef Q_OS_WIN
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK hHook;
    static KeyboardHook *instance;
#endif
};

#endif // KEYBOARDHOOK_H
