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
#include <QMainWindow>
#include<QLineEdit>
#include<QMessageBox>
#include<QSystemTrayIcon>
#include<QGridLayout>
#include<QTableWidget>
#include<QCheckBox>
#include <QNetworkInterface>
#include<QObject>
#include<QUdpSocket>

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

public slots:

    void webBlockAktifPasif();

    void listToFile(QString path, QStringList list, QString filename);
    QStringList fileToList(QString path,QString filename);
    QString listGetLine(QStringList list, QString data);
    QStringList listRemove(QStringList list, QString data);

    QString myMessageBox(QString baslik, QString mesaj, QString evet, QString hayir, QString tamam, QMessageBox::Icon icon);
    void cellDoubleClicked(int iRow, int iColumn);
    void hostAddressMacButtonSlot();
    void komutSudoExpect(QString komut,QString username,QString password);
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

     QTableWidget *twl;
     QLabel *gelenKomut;
     QWidget *wid;
    Ui::MainWindow *ui;
    QWidget *ayar();
    QWidget *aw;
    QTabWidget *tw;
    QWidget *giris();
    QWidget *hakkinda();
    QWidget *qww;
    int boy=30;
    int en=30;
    int btsayisi=4;

    bool kilitstate=false;
    bool transparankilitstate=false;
    bool ekranimagestate=false;

     bool webblockstate;
     QString version;
     QString sudoyetki="";
     QString user;
     QString passwordstatus;
     bool status;

    QStringList ayarlst;


    QString localDir;
    QWidget *kutu=nullptr;
    QWidget *pano;
    QGridLayout *layout;
    QLabel *haber;
    QLabel *baslik;
    QTimer *timer;
    QTimer *tcpMesajControlTimer;
    QUdpSocket *udpSocketSendMyDisp = nullptr;
    QUdpSocket *udpSocketGetX11Mesaj = nullptr;

};

#endif // MAINWINDOW_H
