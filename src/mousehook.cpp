#include "mousehook.h"
#include <QDebug>

#ifdef Q_OS_WIN
HHOOK MouseHook::hHook = nullptr;
MouseHook *MouseHook::instance = nullptr;

MouseHook::MouseHook(QObject *parent) : QObject(parent) {
    instance = this;
}

MouseHook::~MouseHook() {
    removeHook();
}

bool MouseHook::installHook() {
    if (hHook)
        return true;

    // Use WH_MOUSE_LL instead of WH_KEYBOARD_LL
    hHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, GetModuleHandle(NULL), 0);

    if (!hHook) {
        qWarning() << "Failed to install mouse hook! Error:" << GetLastError();
        return false;
    }
    return true;
}

void MouseHook::removeHook() {
    if (hHook) {
        UnhookWindowsHookEx(hHook);
        hHook = nullptr;
    }
}

LRESULT CALLBACK MouseHook::LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && instance) {
        MSLLHOOKSTRUCT *pMouse = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);
        QPoint pos(pMouse->pt.x, pMouse->pt.y);
        int result = 0;
        switch (wParam) {
        case WM_LBUTTONDOWN:
            result = emit instance->mousePressed(Qt::LeftButton, pos);
            break;
        case WM_RBUTTONDOWN:
            result = emit instance->mousePressed(Qt::RightButton, pos);
            break;
        case WM_MBUTTONDOWN:
            result = emit instance->mousePressed(Qt::MiddleButton, pos);
            break;
        case WM_LBUTTONUP:
            result = emit instance->mouseReleased(Qt::LeftButton, pos);
            break;
        case WM_RBUTTONUP:
            result = emit instance->mouseReleased(Qt::RightButton, pos);
            break;
        case WM_MBUTTONUP:
            result = emit instance->mouseReleased(Qt::MiddleButton, pos);
            break;
        case WM_MOUSEMOVE:
            result = emit instance->mouseMove(pos);
            break;
        default:
            break;
        }

        // Return 1 to suppress/block the mouse click system-wide:
        // if (wParam == WM_LBUTTONDOWN) return 1;
        if (result == 1) {
            return result;
        }
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}
#endif
