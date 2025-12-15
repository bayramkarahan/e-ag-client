#ifndef MAINWINDOW1_H
#define MAINWINDOW1_H

#include <QMainWindow>
#include <QLabel>
#include "ffmpegplayer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QLabel *videoLabel;
    FFmpegPlayer *player;
};

#endif // MAINWINDOW1_H
