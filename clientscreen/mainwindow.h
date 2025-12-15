#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "multicast_receiver.h"
#include<QLabel>
#include<QImage>
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QString ip, QWidget *parent = nullptr);
    ~MainWindow();
    QString ip;
private:
    QImage prevImage;
    MulticastReceiver *receiver = nullptr;  // ✅ Null ile başlat
    QLabel *sahne = nullptr;           // ✅ Daha açıklayıcı isim
};

#endif // MAINWINDOW_H
