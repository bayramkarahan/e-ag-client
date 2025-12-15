#ifndef INPUTGRABHELPER_H
#define INPUTGRABHELPER_H

#include <QObject>
#include <QDebug>
#include <QX11Info>

// X11 header’ları Qt’den sonra
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

class InputGrabHelper : public QObject
{
    Q_OBJECT

public:
    explicit InputGrabHelper(QObject *parent = nullptr);

    // ---- Grab Fonksiyonları ----
    bool grabKeyboard();
    bool grabMouse();
    bool grabAll();

    // ---- Ungrab Fonksiyonları ----
    void ungrabKeyboard();
    void ungrabMouse();
    void ungrabAll();

private:
    Display *dpy;
    Window root;
};

#endif // INPUTGRABHELPER_H
