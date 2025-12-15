#include "inputgrabhelper.h"

InputGrabHelper::InputGrabHelper(QObject *parent)
    : QObject(parent)
{
    dpy  = QX11Info::display();
    root = QX11Info::appRootWindow();
}

//
// ------------------------- KEYBOARD GRAB -------------------------
//
bool InputGrabHelper::grabKeyboard()
{
    if (!dpy) return false;

    int ret = XGrabKeyboard(dpy, root, True,
                            GrabModeAsync, GrabModeAsync, CurrentTime);

    if (ret != GrabSuccess) {
        qWarning() << "XGrabKeyboard FAILED!";
        return false;
    }

    // Tüm tuş kombinasyonlarını grab et
    for (int keycode = 8; keycode <= 255; keycode++) {
        for (int mod = 0; mod < (1 << 3); mod++) {
            XGrabKey(dpy, keycode, mod, root, True,
                     GrabModeAsync, GrabModeAsync);
        }
    }

    XFlush(dpy);
    qInfo() << "Keyboard grabbed.";
    return true;
}

void InputGrabHelper::ungrabKeyboard()
{
    if (!dpy) return;

    XUngrabKeyboard(dpy, CurrentTime);
    XUngrabKey(dpy, AnyKey, AnyModifier, root);
    XFlush(dpy);

    qInfo() << "Keyboard ungrabbed.";
}

//
// --------------------------- MOUSE GRAB --------------------------
//
bool InputGrabHelper::grabMouse()
{
    if (!dpy) return false;

    int ret = XGrabPointer(
        dpy, root, True,
        ButtonPressMask | ButtonReleaseMask |
            PointerMotionMask | EnterWindowMask |
            LeaveWindowMask | FocusChangeMask,
        GrabModeAsync, GrabModeAsync,
        None,
        None,
        CurrentTime
        );

    if (ret != GrabSuccess) {
        qWarning() << "XGrabPointer FAILED!";
        return false;
    }

    XFlush(dpy);
    qInfo() << "Mouse grabbed.";
    return true;
}

void InputGrabHelper::ungrabMouse()
{
    if (!dpy) return;

    XUngrabPointer(dpy, CurrentTime);
    XFlush(dpy);

    qInfo() << "Mouse ungrabbed.";
}

//
// --------------------------- ALL GRAB ---------------------------
//
bool InputGrabHelper::grabAll()
{
    bool k = grabKeyboard();
    bool m = grabMouse();

    if (k && m) {
        qInfo() << "Keyboard + Mouse successfully grabbed!";
        return true;
    }

    qWarning() << "Input grab FAILED!";
    return false;
}

void InputGrabHelper::ungrabAll()
{
    ungrabKeyboard();
    ungrabMouse();

    qInfo() << "All input ungrabbed.";
}
