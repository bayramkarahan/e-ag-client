#ifndef FFMPEGPLAYER_H
#define FFMPEGPLAYER_H

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

#include <QThread>
#include <QLabel>
#include <QAudioOutput>
#include <QElapsedTimer>

class FFmpegPlayer : public QThread {
    Q_OBJECT
public:
    explicit FFmpegPlayer(QLabel *outputLabel, QObject *parent = nullptr);
    ~FFmpegPlayer();
    void run() override;
    void stop();
    void setVolume(qreal volume);  // 0.0 - 1.0 arası değer
    void pause();
    void resume();
    bool isPaused() const;

private:
    std::atomic<bool> paused = false;
    volatile bool running = true;
    QLabel *label;

    int videoStreamIndex = -1;
    int audioStreamIndex = -1;

    AVFormatContext *fmtCtx = nullptr;
    AVCodecContext *videoCodecCtx = nullptr;
    AVCodecContext *audioCodecCtx = nullptr;

    SwsContext *swsCtx = nullptr;
    SwrContext *swrCtx = nullptr;

    QAudioOutput *audioOutput = nullptr;
    QIODevice *audioIODevice = nullptr;

    QElapsedTimer timer;

    void initAudioOutput();
};

#endif // FFMPEGPLAYER_H
