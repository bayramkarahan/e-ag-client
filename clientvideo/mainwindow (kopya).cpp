#include "mainwindow1.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(640, 480);
    setCentralWidget(videoLabel);

    player = new FFmpegPlayer(videoLabel, this);
    player->start();
}

MainWindow::~MainWindow() {
    player->stop();
}
