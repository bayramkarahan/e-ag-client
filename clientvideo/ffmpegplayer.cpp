#include "ffmpegplayer.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QThread>

FFmpegPlayer::FFmpegPlayer(QLabel *outputLabel, QObject *parent)
    : QThread(parent), label(outputLabel) {}

FFmpegPlayer::~FFmpegPlayer() {
    stop();
    wait();  // thread bittikten sonra devam et

    // Şimdi güvenle delete yap
    if (audioOutput) {
        audioOutput->stop();
        delete audioOutput;
        audioOutput = nullptr;
    }

    if (swrCtx) {
        swr_free(&swrCtx);
        swrCtx = nullptr;
    }

    if (audioCodecCtx) {
        avcodec_free_context(&audioCodecCtx);
        audioCodecCtx = nullptr;
    }

    if (videoCodecCtx) {
        avcodec_free_context(&videoCodecCtx);
        videoCodecCtx = nullptr;
    }
}

void FFmpegPlayer::stop() {
    requestInterruption();
    running = false;
    paused = false;

    if (fmtCtx) {
        avformat_close_input(&fmtCtx);
        fmtCtx = nullptr;
    }
}

void FFmpegPlayer::initAudioOutput() {
    QAudioFormat format;
    format.setSampleRate(audioCodecCtx->sample_rate);
    format.setChannelCount(audioCodecCtx->channels);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    audioOutput = new QAudioOutput(format);
    audioOutput->setBufferSize(32768); // Büyük buffer kesilmeleri azaltır
    audioIODevice = audioOutput->start();
}

void FFmpegPlayer::run() {
    avformat_network_init();

    const char *url = "udp://@239.0.0.1:1234";

    if (avformat_open_input(&fmtCtx, url, nullptr, nullptr) != 0) {
        qWarning("Cannot open input");
        return;
    }
    if (avformat_find_stream_info(fmtCtx, nullptr) < 0) {
        qWarning("Cannot find stream info");
        return;
    }

    // Stream indexleri bul
    for (unsigned i = 0; i < fmtCtx->nb_streams; i++) {
        AVCodecParameters *codecpar = fmtCtx->streams[i]->codecpar;
        if (codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoStreamIndex == -1) {
            videoStreamIndex = i;
            const AVCodec *videoCodec = avcodec_find_decoder(codecpar->codec_id);
            videoCodecCtx = avcodec_alloc_context3(videoCodec);
            avcodec_parameters_to_context(videoCodecCtx, codecpar);
            avcodec_open2(videoCodecCtx, videoCodec, nullptr);
        }
        else if (codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioStreamIndex == -1) {
            audioStreamIndex = i;
            const AVCodec *audioCodec = avcodec_find_decoder(codecpar->codec_id);
            audioCodecCtx = avcodec_alloc_context3(audioCodec);
            avcodec_parameters_to_context(audioCodecCtx, codecpar);
            avcodec_open2(audioCodecCtx, audioCodec, nullptr);
        }
    }

    if (videoStreamIndex == -1) {
        qWarning("No video stream found");
        return;
    }

    if (audioStreamIndex != -1) {
        initAudioOutput();

        swrCtx = swr_alloc();
        av_opt_set_channel_layout(swrCtx, "in_channel_layout",
                                  audioCodecCtx->channel_layout ? audioCodecCtx->channel_layout :
                                      av_get_default_channel_layout(audioCodecCtx->channels), 0);
        av_opt_set_channel_layout(swrCtx, "out_channel_layout",
                                  av_get_default_channel_layout(audioCodecCtx->channels), 0);
        av_opt_set_int(swrCtx, "in_sample_rate", audioCodecCtx->sample_rate, 0);
        av_opt_set_int(swrCtx, "out_sample_rate", audioCodecCtx->sample_rate, 0);
        av_opt_set_sample_fmt(swrCtx, "in_sample_fmt", audioCodecCtx->sample_fmt, 0);
        av_opt_set_sample_fmt(swrCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
        swr_init(swrCtx);
    }

    swsCtx = sws_getContext(videoCodecCtx->width, videoCodecCtx->height, videoCodecCtx->pix_fmt,
                            videoCodecCtx->width, videoCodecCtx->height, AV_PIX_FMT_RGB24,
                            SWS_BILINEAR, nullptr, nullptr, nullptr);

    AVFrame *frame = av_frame_alloc();
    AVFrame *rgbFrame = av_frame_alloc();
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, videoCodecCtx->width,
                                            videoCodecCtx->height, 1);
    uint8_t *buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(rgbFrame->data, rgbFrame->linesize, buffer,
                         AV_PIX_FMT_RGB24, videoCodecCtx->width, videoCodecCtx->height, 1);

    AVPacket *packet = av_packet_alloc();

    timer.start(); // Zaman ölçümünü başlat

    while (running && av_read_frame(fmtCtx, packet) >= 0) {
        // Eğer pause aktifse, küçük bir uyku ile bekle
        while (paused && running) {
            msleep(10);
        }
        if (packet->stream_index == videoStreamIndex) {
            avcodec_send_packet(videoCodecCtx, packet);
            while (avcodec_receive_frame(videoCodecCtx, frame) == 0) {
                double pts = frame->best_effort_timestamp * av_q2d(fmtCtx->streams[videoStreamIndex]->time_base);
                qint64 targetTimeMs = static_cast<qint64>(pts * 1000);

                while (timer.elapsed() < targetTimeMs && running) {
                    msleep(1);
                }

                sws_scale(swsCtx, frame->data, frame->linesize, 0,
                          videoCodecCtx->height, rgbFrame->data, rgbFrame->linesize);

                QImage img(rgbFrame->data[0], videoCodecCtx->width, videoCodecCtx->height,
                           rgbFrame->linesize[0], QImage::Format_RGB888);

                QPixmap pixmap = QPixmap::fromImage(img);
                label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio));
            }
        }
        else if (packet->stream_index == audioStreamIndex && audioCodecCtx) {
            avcodec_send_packet(audioCodecCtx, packet);
            AVFrame *audioFrame = av_frame_alloc();
            while (avcodec_receive_frame(audioCodecCtx, audioFrame) == 0) {
                int outSamples = av_rescale_rnd(
                    swr_get_delay(swrCtx, audioCodecCtx->sample_rate) + audioFrame->nb_samples,
                    audioCodecCtx->sample_rate, audioCodecCtx->sample_rate, AV_ROUND_UP);

                uint8_t **convertedData = nullptr;
                av_samples_alloc_array_and_samples(&convertedData, nullptr,
                                                   audioCodecCtx->channels,
                                                   outSamples,
                                                   AV_SAMPLE_FMT_S16,
                                                   0);

                int convertedSamples = swr_convert(swrCtx, convertedData, outSamples,
                                                   (const uint8_t **)audioFrame->data,
                                                   audioFrame->nb_samples);

                int bufferSize = av_samples_get_buffer_size(nullptr,
                                                            audioCodecCtx->channels,
                                                            convertedSamples,
                                                            AV_SAMPLE_FMT_S16,
                                                            1);

                if (bufferSize > 0 && audioIODevice) {
                    // Ses bufferını doldurmadan önce QAudioOutput tamponunu kontrol et
                    while (audioOutput->bytesFree() < bufferSize && running) {
                        msleep(5);
                    }
                    audioIODevice->write((char*)convertedData[0], bufferSize);
                }

                av_freep(&convertedData[0]);
                av_freep(&convertedData);
                av_frame_unref(audioFrame);
            }
            av_frame_free(&audioFrame);
        }
        av_packet_unref(packet);
    }

    av_packet_free(&packet);
    av_frame_free(&frame);
    av_frame_free(&rgbFrame);
    av_free(buffer);

    if (swsCtx) sws_freeContext(swsCtx);
    if (audioCodecCtx) avcodec_free_context(&audioCodecCtx);
    if (videoCodecCtx) avcodec_free_context(&videoCodecCtx);
    if (swrCtx) swr_free(&swrCtx);
    if (fmtCtx) avformat_close_input(&fmtCtx);
    avformat_network_deinit();
}
void FFmpegPlayer::setVolume(qreal volume) {
    if (audioOutput) {
        audioOutput->setVolume(volume);
    }
}
void FFmpegPlayer::pause() {
    paused = true;
}

void FFmpegPlayer::resume() {
    paused = false;
}

bool FFmpegPlayer::isPaused() const {
    return paused;
}
