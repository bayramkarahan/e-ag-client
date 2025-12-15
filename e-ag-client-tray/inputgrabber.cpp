#include "inputgrabber.h"
#include <QApplication>
#include <QDebug>

InputGrabber::InputGrabber(QWidget *parent)
    : QWidget(parent),
    keyboardGrabbed(false),
    mouseGrabbed(false)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    resize(1, 1);
    hide();
}

InputGrabber& InputGrabber::instance()
{
    static InputGrabber inst;
    return inst;
}

bool InputGrabber::grabKeyboardOnly()
{
    if (!keyboardGrabbed) {
        grabKeyboard();          // Qt5 → void
        keyboardGrabbed = true;  // Qt5'te her zaman başarılı kabul edilir
    }

    show();
    qDebug() << "[Grabber] Keyboard grabbed =" << keyboardGrabbed;
    return keyboardGrabbed;
}

bool InputGrabber::grabMouseOnly()
{
    if (!mouseGrabbed) {
        show();
        mouseGrabbed = grabMouse(); // Qt5 → bool
    }

    qDebug() << "[Grabber] Mouse grabbed =" << mouseGrabbed;
    return mouseGrabbed;
}

bool InputGrabber::grabAll()
{
    bool k = grabKeyboardOnly();
    bool m = grabMouseOnly();
    return k && m;
}

void InputGrabber::releaseKeyboardOnly()
{
    if (keyboardGrabbed) {
        releaseKeyboard();
        keyboardGrabbed = false;
    }

    if (!mouseGrabbed)
        hide();

    qDebug() << "[Grabber] Keyboard released.";
}

void InputGrabber::releaseMouseOnly()
{
    if (mouseGrabbed) {
        releaseMouse();
        mouseGrabbed = false;
    }

    if (!keyboardGrabbed)
        hide();

    qDebug() << "[Grabber] Mouse released.";
}

void InputGrabber::releaseAll()
{
    releaseMouseOnly();
    releaseKeyboardOnly();

    hide();
    qDebug() << "[Grabber] All released.";
}
