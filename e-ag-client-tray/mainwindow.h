/*****************************************************************************
 *   Copyright (C) 2020 by Bayram KARAHAN                                    *
 *   <bayramk@gmail.com>                                                     *
 *                                                                           *
 *   This program is free software; you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation; either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *   You should have received a copy of the GNU General Public License       *
 *   along with this program; if not, write to the                           *
 *   Free Software Foundation, Inc.,                                         *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .          *
 *****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#pragma once
// 1️⃣ Qt header’ları önce
#include <QApplication>
#include <QMainWindow>
#include <QLineEdit>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QGridLayout>
#include <QTableWidget>
#include <QCheckBox>
#include <QNetworkInterface>
#include <QUdpSocket>
#include <QFileSystemWatcher>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QFileDialog>
#include <QRegularExpression>
#include <QTimeEdit>
#include <QDate>
#include <QTimer>
#include <QTime>
#include <QMenu>
#include <QCloseEvent>
#include <QProcess>
#include <QAuthenticator>
#include <QDesktopWidget>
#include <QtCore/QCoreApplication>
#include <QObject>
#include <QDebug>

// 2️⃣ Kendi header’ların
#include "Database.h"
#include "ekran.h"

class IpMac
{
public:
    QString ip;
    QString mac;
    QString broadcast;
     QString subnet;

};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
virtual void closeEvent ( QCloseEvent * event );
    /**********************Network Profil*****************************/
    /*bool selectedNetworkProfil;
    QString networkIndex;
    QString networkName;
    QString networkTcpPort;
    QString networkBroadCastAddress;
    QString serverAddress;
    QString ipAddress;
    QString macAddress;
    QString ftpPort;
    QString rootPath;
    bool webblockState;
    bool lockScreenState;
    QString language;*/

public slots:
    void clientConfLoad();
    void udpConsoleGetSlot();
    void networkProfilLoad();
    void tcpMessageControlSlot(QJsonObject json);
    void  WidgetClosed();
    void iconActivated(QSystemTrayIcon::ActivationReason);
    void gizle();
    QString myMessageBox(QString baslik, QString mesaj, QString evet, QString hayir, QString tamam, QMessageBox::Icon icon);
    void hostAddressMacButtonSlot();
    void  widgetShow();
    QString getSessionInfo(QString id, QString parametre);
    QString getSeatId();
    void commandExecute(QString kmt);
    bool stringToBool(const QString& str) {
        return str.toLower() == "true"; // Büyük/küçük harf duyarsız karşılaştırma
    }
public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();


private:
     QString x11user="";
     QString x11display="";

     QString x11mydispresult="";
     QList<IpMac> ipmaclist;
     QCheckBox *webblockcb;
     QString rootusername;
     QString rootpassword;
     QLabel *gelenKomut;
     QWidget *wid;
    Ui::MainWindow *ui;
    QWidget *ayar();
    QWidget *aw;
    QWidget *giris();
    QWidget *hakkinda();
    QWidget *qww;
    int boy=30;
    int en=30;
    int btsayisi=4;

    bool kilitState=false;
    bool kilitStateStart=false;

    bool transparankilitState=false;
    bool transparankilitStateStart=false;

    bool ekranimageState=false;
    bool volumeState=true;


     QString version;
     QString sudoyetki="";
     QString user;
     QString passwordstatus;
     bool status;

    QStringList ayarlst;
    QTimer *timergizle;
    QSystemTrayIcon* trayIcon;
    QMenu* trayIconMenu;

    QMenu* createMenu();

    QString localDir;
    QWidget *kutu=nullptr;
    QWidget *pano;
    QGridLayout *layout;
    QLabel *haber;
    QLabel *baslik;
    QTimer *timer;
    QTimer *tcpMesajControlTimer;
    //Ekran *ekran;
    QList<Ekran*> ekranList;

    QUdpSocket *udpConsoleGet = nullptr;
    QUdpSocket *udpConsoleSend= nullptr;
    QString serverIp;
    int sendConsoleCount=0;
    QFileSystemWatcher clientConfWather;
    //InputGrabHelper *inputGrab;
};

#endif // MAINWINDOW_H
