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
#include "mainwindow.h"
#include "inputgrabhelper.h"
#include <windowCloser.h>
InputGrabHelper *inputGrab;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
 {
     inputGrab = new InputGrabHelper(this);
   localDir="/usr/share/e-ag/";
   clientConfWather.addPath(localDir+"clientConf.json");
   connect(&clientConfWather, &QFileSystemWatcher::fileChanged, this,
           [this](){
               clientConfLoad();   // burada tekrar addPath() çağırılacak
           });
   clientConfLoad();
   QProcess process;
   process.start("/bin/bash", {"-c", "dpkg -s e-ag-client | grep -i '^Version:' | awk '{print $2}'"});
   process.waitForFinished();

   trayVersion = QString::fromUtf8(process.readAll()).trimmed();

    udpConsoleGet=new QUdpSocket();
    udpConsoleSend = new QUdpSocket();

    udpConsoleGet->bind(51512, QUdpSocket::ShareAddress);
    QObject::connect(udpConsoleGet,&QUdpSocket::readyRead,[&](){udpConsoleGetSlot();});
    /*******************************************************************/
    webblockcb= new QCheckBox("Her Açılışta Web Sitelerini Engelle.");

    gelenKomut=new QLabel("-------------------");

    trayIcon=new QSystemTrayIcon(this);
    this->resize(340,300);
    setFixedWidth(400);
    setFixedHeight(400);
    setWindowTitle("e-ag-client-tray");
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()/2 - this->width()/2);
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    this->setStyleSheet("background-color: #dfdfdf;");


      //*******************tray***********************************/
      // Tray icon menu
      auto menu = this->createMenu();
      this->trayIcon->setContextMenu(menu);

      // App icon
      auto appIcon = QIcon(":/icons/e-ag-client.svg");
      this->trayIcon->setIcon(appIcon);
      this->setWindowIcon(appIcon);

      // Displaying the tray icon
      this->trayIcon->show();     // Note: without explicitly calling show(), QSystemTrayIcon::activated signal will never be emitted!

      // Interaction
      connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);


      timergizle = new QTimer(this);
      connect(timergizle, SIGNAL(timeout()), this, SLOT(gizle()));
      timergizle->start(1);
      QString seatId=getSeatId();
        x11user=getSessionInfo(seatId,"USER=");
        QStringRef _sessionUser=x11user.rightRef(x11user.length()-5);
        x11user=_sessionUser.toString();


        x11display=getSessionInfo(seatId,"DISPLAY=:");
        QStringRef _sessionDisplay=x11display.rightRef(x11display.length()-9);
        x11display=_sessionDisplay.toString();
        /******************************************************/
        if(!x11display.contains("0", Qt::CaseInsensitive))//!=0
        {
             QString kmt20="nohup /usr/bin/x11vnc -forever -loop -noxdamage -repeat -rfbauth /usr/bin/x11vncpasswd -rfbport 5901 -shared &";
             system(kmt20.toStdString().c_str());
        }
        //sendConsoleCount=0;
        QTimer *udpSocketSendConsoleTimer = new QTimer();
        QObject::connect(udpSocketSendConsoleTimer, &QTimer::timeout, [this,udpSocketSendConsoleTimer](){
            QJsonObject json;
            json["tray_user"] = x11user; //qstring
            json["tray_display"] = x11display; //qstring
            json["tray_volume"] = volumeState; //bool
            json["tray_lock"] = kilitState; //bool
            json["tray_tlock"] = transparankilitState; //bool
            json["tray_ekranimage"] = ekranimageState;  //bool
            json["tray_Version"] = trayVersion;  //bool
            QJsonDocument doc(json);
            QByteArray datagram = doc.toJson(QJsonDocument::Compact);

            udpConsoleSend->writeDatagram(datagram, QHostAddress::LocalHost, 51511);
            qDebug() << "Tray → Console JSON gönderildi:" << datagram;

/*
            if(volumeState==false)
            {
                QJsonObject sendJson{
                    {"mainmessagetype", "x11command"},
                    {"submessagetype",  "volumeoff"},
                    {"mission",         ""},
                    {"missionmessage",  "volume-off"},
                    {"messagevisible",  "1"},
                    {"server_address",  ""},
                    {"port",            ""}
                };
                //tcpMessageControlSlot(QString("x11command|volumeoff||volume-off|1||"));
                tcpMessageControlSlot(sendJson);
            }*/
            if(kilitState==true&&kilitStateStart==false)
                {
                QJsonObject sendJson{
                    {"mainmessagetype", "x11command"},
                    {"submessagetype",  "kilitstatetrue"},
                    {"mission",         ""},
                    {"missionmessage",  "kilit-on"},
                    {"messagevisible",  "0"},
                    {"server_address",  ""},
                    {"port",            ""}
                };
                //tcpMessageControlSlot(QString("x11command|kilitstatetrue||kilit-on|0||"));
                tcpMessageControlSlot(sendJson);
            }
            if(transparankilitState==true&&transparankilitStateStart==false)
            {
                QJsonObject sendJson{
                    {"mainmessagetype", "x11command"},
                    {"submessagetype",  "transparankilitstatetrue"},
                    {"mission",         ""},
                    {"missionmessage",  "kilit-on"},
                    {"messagevisible",  "1"},
                    {"server_address",  ""},
                    {"port",            ""}
                };
                //tcpMessageControlSlot(QString("x11command|transparankilitstatetrue||kilit-off|1||"));
                 tcpMessageControlSlot(sendJson);
            }


        });
        udpSocketSendConsoleTimer->start(5000);

       }

 void MainWindow::networkProfilLoad()
 {

     DatabaseHelper *db=new DatabaseHelper(localDir+"e-ag.json");
     //QJsonArray dizi=db->Oku();
     QJsonArray dizi=db->Ara("selectedNetworkProfil",true);
     if(dizi.count()>0)
     {
         //qDebug()<<"Kayıtlı Host.";
         QJsonObject veri=dizi.first().toObject();
         //qDebug()<<"Yüklenen Ağ Profili:" <<veri;
        /* this->networkIndex=veri["networkIndex"].toString();
         this->selectedNetworkProfil=veri["selectedNetworkProfil"].toBool();
         this->networkName=veri["networkName"].toString();
         this->networkTcpPort=veri["networkTcpPort"].toString();
         this->networkBroadCastAddress=veri["networkBroadCastAddress"].toString();
         this->serverAddress=veri["serverAddress"].toString();
         this->ipAddress=veri["ipAddress"].toString();;
         this->macAddress=veri["macAddress"].toString();;
         this->ftpPort=veri["ftpPort"].toString();
         this->rootPath=veri["rootPath"].toString();
         this->language=veri["language"].toString();
         this->lockScreenState=veri["lockScreenState"].toBool();
         this->webblockState=veri["webblockState"].toBool();*/

     }else
     {
         qDebug()<<"Yeni Network Ekleniyor.";
         hostAddressMacButtonSlot();
         for(int i=0;i<ipmaclist.count();i++)
         {
         //qDebug()<<"broadcast address:"<<i<<ipmaclist[i].broadcast;
         QJsonObject veri;
         veri["networkIndex"] =QString::number(db->getIndex("networkIndex"));
         veri["selectedNetworkProfil"] =true;
         veri["networkName"] = "network";
         veri["networkTcpPort"] = "7879";
         veri["serverAddress"]="";
         veri["ipAddress"]=ipmaclist[i].ip;
         veri["macAddress"]=ipmaclist[i].mac;
         veri["networkBroadCastAddress"]=ipmaclist[i].broadcast;
         veri["ftpPort"]="12345";
         veri["rootPath"]="/tmp/";
         veri["language"]="tr_TR";
         veri["lockScreenState"]=false;
         veri["webblockState"]=false;
         db->Ekle(veri);
         }
         networkProfilLoad();
     }
 }

void MainWindow::udpConsoleGetSlot()
{
    QJsonObject getJson;
    QByteArray datagram;
    while (udpConsoleGet->hasPendingDatagrams()) {
        datagram.resize(int(udpConsoleGet->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udpConsoleGet->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        //QString rmesaj=datagram.constData();
        /***************************************/
        // JSON parse
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(datagram, &parseError);
        if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();
            getJson = obj; // JSON'u direkt sakla
            ///qDebug() << "Tray bilgisi alındı:" << trayEnv;
        } else {
            qWarning() << "Tray JSON parse hatası:" << parseError.errorString();
            return;
        }
        /***************************************/
        tcpMessageControlSlot(getJson);
    }
}

void MainWindow::tcpMessageControlSlot(QJsonObject json)
{
        QString mainmessagetype = json["mainmessagetype"].toString();
        QString submessagetype = json["submessagetype"].toString();
        QString komut = json["mission"].toString();
        qDebug()<<"Gelen Mesaj:"<<json;
        gelenKomut->setText(submessagetype+"  "+komut);

        if(submessagetype=="ekranmesaj")
        {
            qDebug()<<"ekranmesaj:"<<submessagetype<<komut;
            for (Ekran* item : ekranList) {
                delete item;
            }
            ekranList.clear();
            auto ekran=new Ekran();
            ekranList.append(ekran);
            ekran->setWindowFlags(Qt::Tool);
            ekran->ekranMesaj("Yönetici Mesajı:",komut);
            ekran->show();

        }
        else if(submessagetype=="vncviewer")
        {
            //qDebug()<<"mesaj:"<<lst[1]<<lst[2]<<lst[3];
            system("pkill vncviewer &");
            system("sleep 1");
           // QString komut="nohup "+komut+" &";
            //system(komut.toStdString().c_str());
            commandExecute(komut);
        }
        else if(submessagetype=="kilitstatetrue")
        {
            qDebug()<<"ekrankilittrue:"<<submessagetype;
            //ekran->close();
            ///if (!kilitState)
            ///{
                kilitStateStart=true;
                kilitState=true;
                transparankilitState=false;
                ekranimageState=false;
                for ( Ekran *item : ekranList) {
                    delete item;
                }
                ekranList.clear();
                auto ekran=new Ekran();
                ekranList.append(ekran);
                ekran->setWindowFlags(Qt::Tool);
                ekran->ekranKilit("Yönetici Mesajı:","Ekran Kilitlendi!");
                ekran->show();

                ekran->manageCtrlAltDelete(true);
                inputGrab->grabAll();
            ///}

        }
        else if(submessagetype=="kilitstatefalse")
        {
            qDebug()<<"ekrankilitac:"<<submessagetype;
            for ( Ekran *item : ekranList) {
                item->manageCtrlAltDelete(false);
                delete item;
            }
            ekranList.clear();
            //ekran->close();
            kilitState=false;
            inputGrab->ungrabAll();
        }
        else if(submessagetype=="transparankilitstatetrue")
        {
            qDebug()<<"kilittransparanstatetrue";
            ///if (!transparankilitState)
            ///{
                transparankilitStateStart=true;
                kilitState=false;
                transparankilitState=true;
                ekranimageState=false;
                for ( Ekran *item : ekranList) {
                    delete item;
                }
                ekranList.clear();
                auto ekran=new Ekran();
                ekranList.append(ekran);
                ekran->setWindowFlags(Qt::Tool);
                ekran->ekranTransparanKilit();
                ekran->show();
                ekran->manageCtrlAltDelete(true);
                inputGrab->grabAll();
            ///}

        }
        else if(submessagetype=="transparankilitstatefalse")
        {
            qDebug()<<"ekrantransparankilitac:"<<submessagetype;
            for ( Ekran *item : ekranList) {
                item->manageCtrlAltDelete(false);
                delete item;
            }
            ekranList.clear();
            //ekran->close();
            transparankilitState=false;
            inputGrab->ungrabAll();
        }
        else if(submessagetype=="x11command")
        {
            //QString komut="nohup "+lst[2]+" &";
            //system(komut.toStdString().c_str());
            commandExecute(komut);
        }
        else if(submessagetype=="consolecommand")
        {
            for (const Ekran *item : ekranList) {
                delete item;
            }
            ekranList.clear();
            Ekran *ekran=new Ekran();
            ekran->command=komut;
            ekran->commandDetail=json["missiondetail"].toString();
            ekran->commandState=json["missionstate"].toString();
            ekran->commandMessage=json["missionmessage"].toString();
            ekran->commandVisible=json["messagevisible"].toString();
            //ekran->setWindowFlags(Qt::Tool);
            ekran->ekranKomutMesaj();
            qDebug()<<"mesaj gösteriliyor";
            ekran->show();
            ekranList.append(ekran);
            }
        else if(submessagetype == "volumeoff")
        {
            volumeState = false;

            if(QFile::exists("/usr/bin/wpctl")) // PipeWire
            {
                QProcess::execute("systemctl", {"--user", "stop", "pipewire"});
                QProcess::execute("systemctl", {"--user", "stop", "pipewire-pulse"});
                QProcess::execute("systemctl", {"--user", "mask", "pipewire"});
                QProcess::execute("systemctl", {"--user", "mask", "pipewire-pulse"});
            }
            else if(QFile::exists("/usr/bin/pactl")) // PulseAudio
            {
                QProcess::execute("systemctl", {"--user", "stop", "pulseaudio.service"});
                QProcess::execute("systemctl", {"--user", "mask", "pulseaudio.service"});
            }
        }
        else if(submessagetype == "volumeon")
        {
            volumeState = true;

            if(QFile::exists("/usr/bin/wpctl")) // PipeWire
            {
                QProcess::execute("systemctl", {"--user", "unmask", "pipewire"});
                QProcess::execute("systemctl", {"--user", "unmask", "pipewire-pulse"});
                QProcess::execute("systemctl", {"--user", "start", "pipewire"});
                QProcess::execute("systemctl", {"--user", "start", "pipewire-pulse"});
            }
            else if(QFile::exists("/usr/bin/pactl")) // PulseAudio
            {
                QProcess::execute("systemctl", {"--user", "unmask", "pulseaudio.service"});
                QProcess::execute("systemctl", {"--user", "start", "pulseaudio.service"});
            }
        }
        else if(submessagetype=="videoyayinbaslat")
        {
            qDebug()<<"komut:"<<submessagetype<<komut;


        }
        else if(submessagetype=="videoyayindurdur")
        {

        }
        else if(submessagetype=="keyboardoff"){
             inputGrab->grabKeyboard();
            commandExecute("xdotool key Escape");
        }
        else if(submessagetype=="keyboardon"){

            inputGrab->ungrabKeyboard();
        }
        else if(submessagetype=="mouseoff"){
            inputGrab->grabMouse();

        }
        else if(submessagetype=="mouseon"){
            inputGrab->ungrabMouse();
        }
        else if(submessagetype=="keyboardmouseoff"){
            inputGrab->grabAll();
            commandExecute("xdotool key Escape");
        }
        else if(submessagetype=="keyboardmouseon"){
            inputGrab->ungrabAll();
        }
        else if(submessagetype=="windowclose"){
            WindowCloser wc;
            wc.closeDesktopWindows();
        }
/*
 *
        else if(submessagetype=="volumeoff")
        {
             if(QFile::exists("/usr/bin/wpctl"))
            {
                volumeState=false;
                QString komut="nohup wpctl set-mute @DEFAULT_AUDIO_SINK@ 1 &";
                system(komut.toStdString().c_str());
            }
            if(QFile::exists("/usr/bin/pactl"))
            {
                volumeState=false;
                QString komut1="nohup pactl set-sink-mute @DEFAULT_SINK@ 1 &";
                system(komut1.toStdString().c_str());
            }

        }
        else if(submessagetype=="volumeon")
        {
             if(QFile::exists("/usr/bin/wpctl"))
            {
                QString komut="nohup wpctl set-mute @DEFAULT_AUDIO_SINK@ 0 &";
                system(komut.toStdString().c_str());
                volumeState=true;
            }
            if(QFile::exists("/usr/bin/pactl"))
            {
                QString komut1="nohup pactl set-sink-mute @DEFAULT_SINK@ 0 &";
                system(komut1.toStdString().c_str());
                volumeState=true;
            }
        }
*/


}

void MainWindow::commandExecute(QString kmt)
{
    // 1️⃣ Komutu boşluklara göre böl
    QStringList parts = kmt.split(' ', Qt::SkipEmptyParts);

    if(parts.isEmpty()) {
        qDebug() << "Komut boş!";
        return;
    }

    // 2️⃣ İlk eleman program, geri kalanı argümanlar
    QString program = parts.takeFirst();
    QStringList args = parts;

    qint64 pid;
    bool ok = QProcess::startDetached(program, args, QString(), &pid);

    if(ok)
        qDebug() << "Program started, PID:" << pid;
    else
        qDebug() << "Failed to start program";
}

void MainWindow::clientConfLoad()
{
    DatabaseHelper *db=new DatabaseHelper(localDir+"clientConf.json");
    QJsonArray dizi=db->Oku();
    if(dizi.count()>0)
    {
        QJsonObject veri = dizi.first().toObject();
        kilitState=veri.value("kilitState").toBool(false);
        transparankilitState=veri.value("transparankilitState").toBool(false);
        ekranimageState=veri.value("ekranimageState").toBool(false);
        volumeState=veri.value("volumeState").toBool(true);
        }
    clientConfWather.addPath(localDir+"clientConf.json");
}

QString MainWindow::myMessageBox(QString baslik, QString mesaj, QString evet, QString hayir, QString tamam, QMessageBox::Icon icon)
{
    Qt::WindowFlags flags = 0;
    flags |= Qt::Dialog;
    flags |= Qt::X11BypassWindowManagerHint;

    QMessageBox messageBox(this);
    messageBox.setWindowFlags(flags);
    messageBox.setText(baslik+"\t\t\t");
    messageBox.setInformativeText(mesaj);
    QAbstractButton *evetButton;
    QAbstractButton *hayirButton;
    QAbstractButton *tamamButton;

    if(evet=="evet") evetButton =messageBox.addButton(tr("Evet"), QMessageBox::ActionRole);
    if(hayir=="hayir") hayirButton =messageBox.addButton(tr("Hayır"), QMessageBox::ActionRole);
    if(tamam=="tamam") tamamButton =messageBox.addButton(tr("Tamam"), QMessageBox::ActionRole);

    messageBox.setIcon(icon);
    messageBox.exec();
    if(messageBox.clickedButton()==evetButton) return "evet";
    if(messageBox.clickedButton()==hayirButton) return "hayır";
    if(messageBox.clickedButton()==tamamButton) return "tamam";
    return "";
}

MainWindow::~MainWindow()
{
     inputGrab->ungrabAll();
  //  delete ui;
}

QString MainWindow::getSessionInfo(QString id, QString parametre)
{
    QString tempsessionParametre;
    QString filename="/run/systemd/sessions/"+id;

    if(QFile::exists(filename))
    {
        const int size = 256;
        FILE* fp = fopen(filename.toStdString().c_str(), "r");
        if(fp == NULL)
        {
            perror("Error opening /run/systemd/sessions/");
        }

        char line[size];
        fgets(line, size, fp);    // Skip the first line, which consists of column headers.
        while(fgets(line, size, fp))
        {
            QString satir=line;
            satir.chop(1);
            //   tempsessionlist<<satir;
            //qDebug()<<"satir: "<<satir;
            if(satir.contains(parametre)){
                tempsessionParametre=satir;
            }
        }

        fclose(fp);
    }

    return tempsessionParametre;
}

QString  MainWindow::getSeatId()
{
    QString tempseatId;
    if(QFile::exists("/run/systemd/seats/seat0"))
    {
        QStringList list;
        const int size = 256;
        //seat=fileToList("/run/systemd/seats","seat0");
        //qDebug()<<"seat:"<<seat;
        FILE* fp = fopen("/run/systemd/seats/seat0", "r");
        if(fp == NULL)
        {
            perror("Error opening /run/systemd/seats/seat0");
        }

        char line[size];
        fgets(line, size, fp);    // Skip the first line, which consists of column headers.
        while(fgets(line, size, fp))
        {
            QString satir=line;
            satir.chop(1);
            if(satir.contains("ACTIVE=")){
                QStringRef _seatid=satir.rightRef(satir.length()-7);
                tempseatId=_seatid.toString();
                //qDebug()<<seatId;
            }

        }

        fclose(fp);
    }

    return tempseatId;
}

void MainWindow::hostAddressMacButtonSlot()
{
    QHostAddress localhost = QHostAddress(QHostAddress::LocalHost);
ipmaclist.clear();
    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
           foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
               QHostAddress *hostadres=new QHostAddress(entry.ip().toString());
               if(hostadres->protocol() == QAbstractSocket::IPv4Protocol &&
                       !hostadres->isLoopback() )
               {
                  IpMac im;
                  im.ip=entry.ip().toString();
                  im.mac=networkInterface.hardwareAddress();
                  im.broadcast=entry.broadcast().toString();
                  im.subnet=entry.netmask().toString();
                  ipmaclist.append(im);
                 // qDebug()<<"mac:"<<networkInterface.hardwareAddress();
                  //qDebug()<<"ip  address:"<<entry.ip().toString();
                 // qDebug()<<"broadcast  address:"<<entry.broadcast().toString();
              }
           }
       }



}

void  MainWindow::gizle()
{
       QWidget::hide();
    timergizle->stop();

  }
void  MainWindow::widgetShow()
{

    qDebug()<<"ekranı göster";
    system("sh -c \"pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY e-ag-client-gui\"");
 }

QMenu* MainWindow::createMenu()
{

    auto minimizeAction = new QAction(tr("Gi&zle"), this);
    connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);
    auto restoreAction = new QAction(tr("&Ayarlar"), this);
    connect( restoreAction, SIGNAL(triggered()), this, SLOT(widgetShow()));
    auto menu = new QMenu(this);
    menu->addAction(minimizeAction);
    //menu->addAction(maximizeAction);
    menu->addAction(restoreAction);
    menu->addSeparator();

    return menu;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason_)
{
  switch (reason_) {
  case QSystemTrayIcon::Trigger:
   // this->trayIcon->showMessage("e-ag", "İstemci Uygulaması!");
        widgetShow();
    break;
  default:
    ;
  }
}

void MainWindow::WidgetClosed()
{

    QWidget::hide();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
     emit WidgetClosed();
     //QWidget::hide();
     event->ignore();

}
