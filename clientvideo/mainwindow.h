#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QToolButton>
#include <QSlider>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ffmpegplayer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void togglePlayPause();
    void setVolume(int value);
    void seek(int position);

private:
    QLabel *videoLabel;
    FFmpegPlayer *player;

    QToolButton *playPauseButton;
    QSlider *volumeSlider;
    QSlider *timelineSlider;

    bool isPlaying = false;
};

#endif // MAINWINDOW_H
