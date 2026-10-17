#include "keyboardhook.h"
#include <QDebug>

#ifdef Q_OS_WIN
HHOOK KeyboardHook::hHook = nullptr;
KeyboardHook *KeyboardHook::instance = nullptr;

KeyboardHook::KeyboardHook(QObject *parent) : QObject(parent) {
    instance = this;
}

KeyboardHook::~KeyboardHook() {
    removeHook();
}

bool KeyboardHook::installHook() {
    if (hHook)
        return true; // Already installed

    // WH_KEYBOARD_LL passes NULL for hInstance when using a thread message loop
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);

    if (!hHook) {
        qWarning() << "Failed to install keyboard hook! Error:" << GetLastError();
        return false;
    }
    return true;
}

void KeyboardHook::removeHook() {
    if (hHook) {
        UnhookWindowsHookEx(hHook);
        hHook = nullptr;
    }
}

LRESULT CALLBACK KeyboardHook::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && instance) {
        KBDLLHOOKSTRUCT *pKey = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            emit instance->keyPressed(pKey->vkCode);
        } else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            emit instance->keyReleased(pKey->vkCode);
        }

        // Return non-zero to suppress/block the key from reaching other applications:
        // if (pKey->vkCode == VK_F1) return 1;
    }

    // Pass the hook event to the next handler in the chain
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}
#endif