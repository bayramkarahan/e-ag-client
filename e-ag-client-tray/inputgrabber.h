#ifndef INPUTGRABBER_H
#define INPUTGRABBER_H

#include <QWidget>

class InputGrabber : public QWidget
{
    Q_OBJECT

public:
    explicit InputGrabber(QWidget *parent = nullptr);
    static InputGrabber& instance();

    bool grabKeyboardOnly();
    bool grabMouseOnly();
    bool grabAll();

    void releaseKeyboardOnly();
    void releaseMouseOnly();
    void releaseAll();

private:
    bool keyboardGrabbed;
    bool mouseGrabbed;
};

#endif // INPUTGRABBER_H
