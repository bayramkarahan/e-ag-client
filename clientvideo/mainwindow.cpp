#include "mainwindow.h"
#include <QIcon>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(640, 480);

    playPauseButton = new QToolButton(this);
    playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
    connect(playPauseButton, &QToolButton::clicked, this, &MainWindow::togglePlayPause);

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(100);  // Max ses başlangıçta
    connect(volumeSlider, &QSlider::valueChanged, this, &MainWindow::setVolume);

    timelineSlider = new QSlider(Qt::Horizontal, this);
    timelineSlider->setRange(0, 1000);  // Örnek max, daha sonra video süresine göre ayarla
    connect(timelineSlider, &QSlider::sliderMoved, this, &MainWindow::seek);

    // Layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(videoLabel);

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->addWidget(playPauseButton);
    controlsLayout->addWidget(volumeSlider);
    mainLayout->addWidget(timelineSlider);
    mainLayout->addLayout(controlsLayout);

    setCentralWidget(centralWidget);

    player = new FFmpegPlayer(videoLabel, this);
    connect(player, &QThread::finished, player, &QObject::deleteLater);
    player->start();
    isPlaying = true;
}

MainWindow::~MainWindow() {
    if (player) {
        player->stop();
        player->wait();
        player = nullptr;  // deleteLater Qt tarafından yapılacak
    }
}

void MainWindow::togglePlayPause() {
    if (isPlaying) {
        player->pause();
        playPauseButton->setIcon(QIcon::fromTheme("media-playback-start"));
        isPlaying = false;
    } else {
        player->resume();
        playPauseButton->setIcon(QIcon::fromTheme("media-playback-pause"));
        isPlaying = true;
    }
}


void MainWindow::setVolume(int value) {
    if (player) {
        player->setVolume(value / 100.0);  // Doğrudan public fonksiyonu kullan
    }
}

// Timeline kaydırıcısı hareket ettiğinde
void MainWindow::seek(int position) {
    // FFmpegPlayer'da seek fonksiyonunu yazmalısın.
    // position parametresi ile frame veya saniye bazlı atlama yapılabilir.
}

