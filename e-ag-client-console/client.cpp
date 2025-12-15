#include "client.h"
#include<QDebug>
#include<QProcess>
#include <QtCore/QTimer>
#include <stdio.h>
#include <QtCore/QCoreApplication>
#include<Database.h>
#include<userprivilegehelper.h>
Client::Client()
{
    localDir="/usr/share/e-ag/";
    localDir1="/tmp/";
    clientConfWather.addPath(localDir+"clientConf.json");
    connect(&clientConfWather, &QFileSystemWatcher::fileChanged, this,
            [this](){
                clientConfLoad();   // burada tekrar addPath() çağırılacak
            });

    hostAddressMacButtonSlot();
/************************************************/
    tcpMesajSendTimer = new QTimer();
    QObject::connect(tcpMesajSendTimer, &QTimer::timeout, [&](){
        tcpMesajSendTimerSlot(false,"","","");
    });
    tcpMesajSendTimer->start(7000);

    networkProfilLoad();

    for (const NetProfil &item : NetProfilList) {
        if (item.serverAddress=="") continue;
        if (item.selectedNetworkProfil==false) continue;
        // if(item.webblockState) webBlockAktifPasif(true);
    }

//ilk yüklenen ayarlar
    clientConfLoad();
    qint64 pid = 0;
    if(mouseState)
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/bin/input-mouse-on", QString(), &pid);
    else
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/bin/input-mouse-off", QString(), &pid);
    if(keyboardState)
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/bin/input-keyboard-on", QString(), &pid);
    else
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/bin/input-keyboard-off", QString(), &pid);

    if(internetState)
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/share/webblock/script/internet-on", QString(), &pid);
    else
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/share/webblock/script/internet-off", QString(), &pid);

    if(webblockState)
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "systemctl restart webblock.service", QString(), &pid);
    else
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "systemctl stop webblock.service", QString(), &pid);

    if(onlywebblockState)
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/share/webblock/script/only-webblock stop", QString(), &pid);
    else
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/share/webblock/script/only-webblock start", QString(), &pid);

    if(!youtubeState)
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/share/webblock/script/domainblock add youtube.com", QString(), &pid);
    else
        QProcess::startDetached("/bin/bash", QStringList() << "-c" << "/usr/share/webblock/script/domainblock remove youtube.com", QString(), &pid);

    QSysInfo sysinfo;
    hostname = sysinfo.machineHostName();

    trayEnv = QJsonObject();
    trayEnv["tray_user"] = "noLogin";
    trayEnv["tray_display"] = "0";
    trayEnv["tray_volume"] = false;
    trayEnv["tray_lock"] = false;
    trayEnv["tray_tlock"] = false;
    trayEnv["tray_ekranimage"] = false;
   }

void Client::udpServerSendSlot(const QJsonObject &mainJson, bool sendStatus)
{
    if (udpServerGetStatus == true) return;
    hostAddressMacButtonSlot();
    if (udpServerSend == nullptr) {
        qDebug() << "Server bağlı değil! Bağlanıyor...";
        socketBaglama();
    }
    for (const NetProfil &item : NetProfilList) {
        if (item.serverAddress == "") continue;
        if (!item.selectedNetworkProfil) continue;
        // mainJson kopyasını al
        QJsonObject sendJson = mainJson;
        // Network bilgilerini ekle
        sendJson["messagetype"]="eagclientconf";
        sendJson["console_keyboardState"] = keyboardState;
        sendJson["console_mouseState"] = mouseState;
        sendJson["console_internetState"] = internetState;
        sendJson["console_webblockState"] = webblockState;
        sendJson["console_youtubeState"] = youtubeState;
        sendJson["ip_address"] = item.ipAddress;
        sendJson["mac_address"] = item.macAddress;
        /*if(!mouseState){
            QByteArray datagram = QString("x11command|volumeoff||volume-off|1||").toUtf8();
            udpTraySend->writeDatagram(datagram,QHostAddress::LocalHost, 51512);
        }*/
        QByteArray datagram = QJsonDocument(sendJson).toJson(QJsonDocument::Compact);
        udpServerSend->writeDatagram(datagram, QHostAddress(item.serverAddress), item.networkTcpPort.toInt());
        //qDebug() << "server send mesaj (JSON):" << sendJson;
       }
}

void Client::udpGuiGetSlot()
{
    QByteArray datagram;
    QStringList mesaj;

    while (udpGuiGet->hasPendingDatagrams()) {
        datagram.resize(int(udpGuiGet->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;

        udpGuiGet->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString rmesaj=datagram.constData();
        qDebug()<<"udpClientGuiRead:"<<rmesaj;
        clientGuiEnv=rmesaj;
        if(rmesaj=="webblocktrue")
            webBlockAktifPasif(true);

        if(rmesaj=="webblockfalse")
            webBlockAktifPasif(false);
    }
}

void Client::networkProfilLoad()
{

    DatabaseHelper *db=new DatabaseHelper(localDir+"e-ag.json");
    //QJsonArray dizi=db->Oku();
    QJsonArray dizi=db->Ara("selectedNetworkProfil",true);
    NetProfilList.clear();
    if(dizi.count()>0)
    {
        for (const QJsonValue &item : dizi) {
            QJsonObject veri=item.toObject();
            //qDebug()<<"Yüklenen Ağ Profili:" <<veri;
            NetProfil np;
            np.networkIndex=veri["networkIndex"].toString();
            np.selectedNetworkProfil=veri["selectedNetworkProfil"].toBool();
            np.networkName=veri["networkName"].toString();
            np.networkTcpPort=veri["networkTcpPort"].toString();
            np.networkBroadCastAddress=veri["networkBroadCastAddress"].toString();
            np.serverAddress=veri["serverAddress"].toString();
            np.ipAddress=veri["ipAddress"].toString();
            np.macAddress=veri["macAddress"].toString();
            np.ftpPort=veri["ftpPort"].toString();
            np.rootPath=veri["rootPath"].toString();
            np.language=veri["language"].toString();
            np.lockScreenState=veri["lockScreenState"].toBool();
            np.webblockState=veri["webblockState"].toBool();
            NetProfilList.append(np);
        }
    }else{
        qDebug()<<"Yeni Network Ekleniyor.";
        hostAddressMacButtonSlot();
        for(int i=0;i<interfaceList.count();i++)
        {
            //qDebug()<<"broadcast address:"<<i<<ipmaclist[i].broadcast;
            QJsonObject veri;
            veri["networkIndex"] =QString::number(db->getIndex("networkIndex"));
            veri["selectedNetworkProfil"] =true;
            veri["networkName"] = "network";
            veri["networkTcpPort"] = "7879";
            veri["serverAddress"]="";
            veri["ipAddress"]=interfaceList[i].ip;
            veri["macAddress"]=interfaceList[i].mac;
            veri["networkBroadCastAddress"]=interfaceList[i].broadcast;
            veri["ftpPort"]="12345";
            veri["rootPath"]="/tmp/";
            veri["language"]="tr_TR";
            veri["lockScreenState"]=false;
            veri["webblockState"]=false;
            db->Sil("networkBroadCastAddress",interfaceList[i].broadcast);
            db->Ekle(veri);
        }
        //internet yoksa olur
        if(interfaceList.count()==0)
        {
            //qDebug()<<"broadcast address:"<<i<<interfaceList[i].broadcast;
            QJsonObject veri;
            veri["networkIndex"] =QString::number(db->getIndex("networkIndex"));
            veri["selectedNetworkProfil"] =true;
            veri["networkName"] = "networknullip";
            veri["networkTcpPort"] = "7879";
            veri["serverAddress"]="7777";
            veri["ipAddress"]="8888";
            veri["macAddress"]="99999";
            veri["networkBroadCastAddress"]="";
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

void Client::udpServerGetSlot()
{
    QJsonObject getJson;
    udpServerGetStatus=true;
    QByteArray datagram;
    QStringList mesaj;
    while (udpServerGet->hasPendingDatagrams()) {
        datagram.resize(int(udpServerGet->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udpServerGet->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        /***************************************/
        // JSON parse
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(datagram, &parseError);
        if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();
            getJson = obj; // JSON'u direkt sakla
            qDebug() << "Server Mesajı:" << getJson;
        } else {
            qWarning() << "Hatalı Mesaj:" << datagram.constData();

            qWarning() << "Tray JSON parse hatası:" << parseError.errorString();
            return;
        }
        /***************************************/
        QString mainmessagetype = getJson["mainmessagetype"].toString();
        if(mainmessagetype=="webblockserversendfile")
        {
            qDebug()<<"*********************************************************";
            QString dosya=getJson["submessagetype"].toString();
            qDebug()<<"gelen dosya"<<dosya;

            qDebug()<<"*********************************************************";
            QString kmt1="cp /tmp/"+dosya+" /usr/share/e-ag/";
            QString kmt2="chmod 777 /usr/share/e-ag/"+dosya;
            system(kmt1.toStdString().c_str());system("sleep 0.1");
            system(kmt2.toStdString().c_str());system("sleep 0.1");
            bool webblockState=false;
            for (const NetProfil &item : NetProfilList) {
                if (item.serverAddress=="") continue;
                if (item.selectedNetworkProfil==false) continue;
                if(item.webblockState) webblockState=true;
            }
            if(webblockState)
            {
                qDebug()<<"Client webblockState Ayarları:"<<webblockState;
                webBlockAktifPasif(true);
            }
            if(!webblockState)
            {
                qDebug()<<"Client webblockState Ayarları:"<<webblockState;
                webBlockAktifPasif(false);
            }
        }
        else if(mainmessagetype=="debscriptsendfile")
        {
            QString dosya=getJson["submessagetype"].toString();
            qDebug()<<"gelen dosya"<<dosya;
            QString kmt2="chmod 755 /tmp/debeagscript&";
            system(kmt2.toStdString().c_str());
            system("sleep 0.1");
            system("systemctl start debeagscript.service");
        }
        else if(mainmessagetype=="scriptsendfile")
        {
            QString dosya=getJson["submessagetype"].toString();
            qDebug()<<"gelen dosya"<<dosya;
            QString kmt2="chmod 755 /tmp/eagscript&";
            system(kmt2.toStdString().c_str());
            system("sleep 0.1");
            system("systemctl start eagscript.service");
        }
        else if(mainmessagetype=="homesendfile")
        {
            qDebug()<<"*********************************************************";
            QString dosya=getJson["submessagetype"].toString();
            qDebug()<<"gelen dosya"<<dosya;
            qDebug() << "consoleEnv: " << QJsonDocument(consoleEnv).toJson(QJsonDocument::Compact);
            qDebug() << "masaüstü kullanıcısı: " << consoleEnv["console_user"].toString();
            QString guiusername=consoleEnv["console_user"].toString();
            qDebug()<<"*********************************************************";
            QString kmt1="cp /tmp/"+dosya+" /home/"+guiusername+"/";
            QString kmt2="chmod 777 /home/"+guiusername+"/"+dosya;
            QString kmt3="chown "+guiusername+":"+guiusername+" /home/"+guiusername+"/"+dosya;
            system(kmt1.toStdString().c_str());system("sleep 0.1");
            system(kmt2.toStdString().c_str());system("sleep 0.1");
            system(kmt3.toStdString().c_str());
        }
        else if(mainmessagetype=="desktopsendfile")
        {
            qDebug()<<"*********************************************************";
            QString dosya=getJson["submessagetype"].toString();
            qDebug()<<"gelen dosya"<<dosya;
            qDebug()<<"masaüstü kullanıcısı: "<<consoleEnv["console_user"].toString();
            QString guiusername=consoleEnv["console_user"].toString();
            qDebug()<<"*********************************************************";
            QString kmt1="cp /tmp/"+dosya+" /home/"+guiusername+"/Masaüstü/";
            QString kmt2="chmod 777 /home/"+guiusername+"/Masaüstü/"+dosya;
            QString kmt3="chown "+guiusername+":"+guiusername+" /home/"+guiusername+"/Masaüstü/"+dosya;
            system(kmt1.toStdString().c_str());system("sleep 0.1");
            system(kmt2.toStdString().c_str());system("sleep 0.1");
            system(kmt3.toStdString().c_str());
        }
        else if(mainmessagetype=="desktopsendworkfile")
        {
            qDebug()<<"*********************************************************";
            QString dosya=getJson["submessagetype"].toString();
            qDebug()<<"gelen dosya"<<dosya;
            qDebug()<<"masaüstü kullanıcısı: "<<consoleEnv["console_user"].toString();
            QString guiusername=consoleEnv["console_user"].toString();
            qDebug()<<"*********************************************************";
            QString kmt0="mv /home/"+guiusername+"/Masaüstü/"+dosya+" /home/"+guiusername+"/Masaüstü/old.e-ag-server";
            QString kmt1="cp /tmp/"+dosya+" /home/"+guiusername+"/Masaüstü/";
            QString kmt2="chmod 777 /home/"+guiusername+"/Masaüstü/"+dosya;
            QString kmt3="chown "+guiusername+":"+guiusername+" /home/"+guiusername+"/Masaüstü/"+dosya;
            system(kmt0.toStdString().c_str());system("sleep 0.1");
            system(kmt1.toStdString().c_str());system("sleep 0.1");
            system(kmt2.toStdString().c_str());system("sleep 0.1");
            system(kmt3.toStdString().c_str());
        }
        else if(mainmessagetype=="x11command")
        {
            QString submessagetype=getJson["submessagetype"].toString();
            //QByteArray datagram = rmesaj.toUtf8();// +QHostAddress::LocalHost;
            udpTraySend->writeDatagram(datagram,QHostAddress::LocalHost, 51512);
            if(submessagetype=="volumeon")
            {clientConfUpdate("volumeState",true); }
            if(submessagetype=="volumeoff")
            {clientConfUpdate("volumeState",false);}

            if(submessagetype=="kilitstatetrue")
            {clientConfUpdate("kilitState",true); }
            if(submessagetype=="kilitstatefalse")
            {clientConfUpdate("kilitState",false);}

            if(submessagetype=="transparankilitstatetrue")
            {clientConfUpdate("transparankilitState",true); }
            if(submessagetype=="transparankilitstatefalse")
            {clientConfUpdate("transparankilitState",false);}

        }
        else if(mainmessagetype=="consolecommand")
        {
            QString mission=getJson["mission"].toString();
            QString missionmessage=getJson["missionmessage"].toString();
            QString messagevisible=getJson["messagevisible"].toString();

            commandExecuteSlot(mission,missionmessage,messagevisible);
            //QByteArray datagram = rmesaj.toUtf8();// +QHostAddress::LocalHost;
            //udpTraySend->writeDatagram(datagram,QHostAddress::LocalHost, 51512);
        }
        else if(mainmessagetype=="seatlogin")
        {
            // sadece program, argüman yok
            QString komut=getJson["mission"].toString();


            QStringList parts = komut.split(' ', Qt::SkipEmptyParts);
            QString program = parts.takeFirst();
            QStringList args = parts; // boş liste olur

            qint64 pid;
            bool ok = QProcess::startDetached(program, args, QString(), &pid);

            if(ok)
                qDebug() << "Program started, PID:" << pid;
            else
                qDebug() << "Failed to start program";
        }
        else if(mainmessagetype=="dosyatopla")
        {
            QString severip=getJson["server_address"].toString();
            QString guiusername=consoleEnv["console_user"].toString();
            QString consolehostname=consoleEnv["console_hostname"].toString();
            hostAddressMacButtonSlot();//local ip adresi tespit ediliyor.

            QDir directory("/home/"+guiusername+"/Masaüstü");
            QStringList filelist = directory.entryList(QStringList() << "e-ag-server*",QDir::Files);
            QString ad="";
            QString gercekad="";
            foreach(QString filename, filelist) {
                QFileInfo fi(filename);
                ad=fi.fileName();
                QString uzanti = fi.completeSuffix();
                gercekad="/home/"+guiusername+"/Masaüstü\/"+filename;
                /*if(uzanti!="")
                    ad="-e-ag-server."+uzanti;
                else
                    ad="-e-ag-server";*/
                qDebug()<<"dosyalar"<<filename<<gercekad<<ad;

            qDebug()<<"dosyalar"<<gercekad<<ad;

            for (const NetProfil &item : NetProfilList) {
                if (item.serverAddress=="") continue;
                if (item.selectedNetworkProfil==false) continue;

                QString komut="/usr/bin/scd-client "+severip+" 12345 PUT "+gercekad+" /"+consolehostname+"-"+item.ipAddress+"-"+ad;
                // system(komut.toStdString().c_str());
                qDebug()<<"komut: "<<komut;
                qDebug()<<"kopayalanacak gercekad: "<<gercekad<<"kopayalanacak ad: "<<ad;
                qDebug()<<"yeni dosya adı: "<<consolehostname+"-"+item.ipAddress+ad;
                qDebug()<<"guiusername: "<<guiusername;
                QStringList arguments;
                arguments << "-c" << komut;
                QProcess process;
                process.start("/bin/bash",arguments);
                process.waitForFinished(-1); // will wait forever until finished

                if(item.networkBroadCastAddress!=""&&
                    item.serverAddress.section(".",0,2)==item.networkBroadCastAddress.section(".",0,2)&&
                    item.serverAddress.section(".",0,2)==item.ipAddress.section(".",0,2))
                {
                    // mainJson kopyasını al
                    QJsonObject sendJson;
                    sendJson["messagetype"]="sendfileclient";
                    sendJson["ip_address"] = item.ipAddress;
                    sendJson["mac_address"] = item.macAddress;
                    sendJson["filename"] = consolehostname+"-"+item.ipAddress+ad;
                    QByteArray datagram = QJsonDocument(sendJson).toJson(QJsonDocument::Compact);
                    udpServerSend->writeDatagram(datagram,QHostAddress(item.serverAddress), item.networkTcpPort.toInt());
                    ///qDebug()<<msg<<networkTcpPort;
                }
            }
            }
        }
    }
    udpServerGetStatus=false;
}

void Client::udpTrayGetSlot()
{
    while (udpTrayGet->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(int(udpTrayGet->pendingDatagramSize()));

        QHostAddress sender;
        quint16 senderPort;
        udpTrayGet->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        // JSON parse
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(datagram, &parseError);
        if (parseError.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();
            trayEnv = obj; // JSON'u direkt sakla
            ///qDebug() << "Tray bilgisi alındı:" << trayEnv;
        } else {
            ///qWarning() << "Tray JSON parse hatası:" << parseError.errorString();
        }
    }
}
// tcpMesajSendTimerSlot(true, command, stdOut, "0");

void Client::tcpMesajSendTimerSlot(bool commandDetailStatus,QString command,QString commandDetail,QString commandStatus)
{
   UserPrivilegeHelper helper;
    SessionInfo info = helper.getActiveSessionInfo();

   /* if (info.valid) {
        qDebug() << "Kullanıcı:" << info.username;
        qDebug() << "UID/GID:" << info.uid << "/" << info.gid;
        qDebug() << "Home:" << info.home;
        qDebug() << "Display:" << info.display;
        qDebug() << "Type:" << info.type;
        qDebug() << "Service:" << info.service;
    }*/

    consoleEnv = QJsonObject();
    consoleEnv["console_user"] = info.username;
    consoleEnv["console_display"] = info.display;
    consoleEnv["console_uid"] = info.uid;
    consoleEnv["console_hostname"] = hostname;
    consoleEnv["console_desktop_manager"] = info.service;

    //e-ag-tray çalışmıyorsa
    //if(QFile::exists("/tmp/.e-ag-client-tray")==false)
    //{

    //}

    /*if (trayEnv.isEmpty()) {
        trayEnv = QJsonObject();
        trayEnv["tray_user"] = "noLogin";
        trayEnv["tray_display"] = "0";
        trayEnv["tray_volume"] = false;
        trayEnv["tray_lock"] = false;
        trayEnv["tray_tlock"] = false;
        trayEnv["tray_ekranimage"] = false;
    }*/

    // pgrep 15 karakterden fazla olmasın diye tra kullanılmış
    if (!uygulamaCalisiyorMu("pgrep e-ag-client-tra")) {
        trayEnv = QJsonObject();
        trayEnv["tray_user"] = "noLogin";
        trayEnv["tray_display"] = "0";
        trayEnv["tray_volume"] = false;
        trayEnv["tray_lock"] = false;
        trayEnv["tray_tlock"] = false;
        trayEnv["tray_ekranimage"] = false;
    }

    bool sshState = (getIpPortStatus("systemctl status ssh.service | grep 'running' | wc -l", 0) == "open");
    bool xrdpState = (getIpPortStatus("systemctl status xrdp.service | grep 'running' | wc -l", 0) == "open");

    QString vncports = findX11vncPort("netstat -tulnp");

    QJsonObject json;

    for (auto key : trayEnv.keys())
        json[key] = trayEnv[key];

    for (auto key : consoleEnv.keys())
        json[key] = consoleEnv[key];

    json["console_sshstate"] = sshState;
    json["console_xrdpstate"] = xrdpState;
    json["console_vncports"] = vncports;

   /* if (commandDetailStatus) {
        qDebug()<<"komut çıktısı eklendi";
        json["command"] = command;
        json["command_detail"] = commandDetail;
        json["command_status"] = commandStatus;
    }*/

    udpServerSendSlot(json, commandDetailStatus);

    tcpMesajSendTimer->stop();
    tcpMesajSendTimer->start(7000);

}


void Client::socketBaglama()
{

    QString uport="7879";
    if(NetProfilList.count()>0)
        uport=NetProfilList.first().networkTcpPort;
    std::reverse(uport.begin(), uport.end());
    qDebug()<<"SocketBaglama";
    qDebug()<<"Socket bağlantı portu: "<<uport;
    /***********************************/
qDebug()<<"debug socketBaglama2";
    //  QHostAddress *host  = new QHostAddress("192.168.63.254");
    //  QHostAddress *server = new QHostAddress("192.168.23.253");*/

    udpServerSend = new QUdpSocket();
    udpTraySend= new QUdpSocket();
    udpServerGet = new QUdpSocket();
    udpTrayGet=new QUdpSocket();
    udpGuiGet=new QUdpSocket();

    udpServerGet->bind(uport.toInt(), QUdpSocket::ShareAddress);
    udpTrayGet->bind(51511, QUdpSocket::ShareAddress);
    udpGuiGet->bind(51521, QUdpSocket::ShareAddress);

    //udpSocketGet->bind(*host, uport.toInt());
    QObject::connect(udpServerGet,&QUdpSocket::readyRead,[&](){udpServerGetSlot();});
    QObject::connect(udpTrayGet,&QUdpSocket::readyRead,[&](){udpTrayGetSlot();});
    QObject::connect(udpGuiGet,&QUdpSocket::readyRead,[&](){udpGuiGetSlot();});

    qDebug()<<uport<<"udp bağlandı";
    tcpMesajSendTimerSlot(false,"","","");


}

void Client::hostAddressMacButtonSlot()
{
    QHostAddress localhost = QHostAddress(QHostAddress::LocalHost);
interfaceList.clear();
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
                  interfaceList.append(im);
                 // qDebug()<<"mac:"<<networkInterface.hardwareAddress();
                  //qDebug()<<"ip  address:"<<entry.ip().toString();
                  // qDebug()<<"broadcast  address:"<<entry.broadcast().toString();
                  // qDebug()<<"broadcast  address:"<<entry.broadcast().toString();
               ///  qDebug()<<"type:"<<networkInterface.name()<<networkInterface.type();
                QString program="ethtool -s "+networkInterface.name()+" wol g &";
                system(program.toStdString().c_str());

               }
           }
       }


}

void Client::commandExecuteSlot(QString command,QString komutMesaji,QString mesajVisible)
{
    qDebug() << "Çalıştırılan komut:" << command;
    int exitCode;
    QString stdOut;
    QString stdErr;

    connect(&process, &QProcess::readyReadStandardOutput, [&]() {
        stdOut += process.readAllStandardOutput();
    });

    connect(&process, &QProcess::readyReadStandardError, [&]() {
        stdErr += process.readAllStandardError();
    });

    QStringList arguments;
    arguments << "-c" << command;  // "&" kaldırıldı
    process.start("/bin/bash", arguments);

    if (process.waitForFinished(-1)) {  // Sonsuza kadar bekler
        //QString mesaj;
        QJsonObject sendJson;
        exitCode = process.exitCode();  // <-- Eksik olan kısım
        // Çıktıların tamamını burada da al
        stdOut += process.readAllStandardOutput();
        stdErr += process.readAllStandardError();

        if (process.exitStatus() == QProcess::NormalExit && exitCode == 0 && stdErr.isEmpty()) {
            qDebug() << "Başarılı çıktı:" << stdOut;
            //traya gönder
            if(mesajVisible=="1")
            {
                QJsonObject sendTrayJson;
                //QString hatamesaji=stdErr.left(255);
                sendTrayJson["mainmessagetype"] = "consolecommand";
                sendTrayJson["submessagetype"] ="consolecommand" ;
                sendTrayJson["mission"] = command;
                sendTrayJson["missiondetail"] = stdOut;
                sendTrayJson["missionmessage"]=komutMesaji;
                sendTrayJson["messagevisible"]=mesajVisible;
                sendTrayJson["missionstate"] = "0";
                QByteArray datagram = QJsonDocument(sendTrayJson).toJson(QJsonDocument::Compact);
                udpTraySend->writeDatagram(datagram, QHostAddress::LocalHost, 51512);
            }

           //servera gönder
            for (const NetProfil &item : NetProfilList) {
                if (item.serverAddress=="") continue;
                if (item.selectedNetworkProfil==false) continue;
                if(item.networkBroadCastAddress!=""&&
                    item.serverAddress.section(".",0,2)==item.networkBroadCastAddress.section(".",0,2)&&
                    item.serverAddress.section(".",0,2)==item.ipAddress.section(".",0,2))
                {
                    // mainJson kopyasını al
                    QJsonObject sendServerJson;
                    // Network bilgilerini ekle
                    sendServerJson["command"] = command;
                    sendServerJson["command_detail"] = stdOut;
                    sendServerJson["command_status"] = "0";
                    sendServerJson["messagetype"]="commandstateclient";
                    sendServerJson["ip_address"] = item.ipAddress;
                    sendServerJson["mac_address"] = item.macAddress;
                    QByteArray datagram = QJsonDocument(sendServerJson).toJson(QJsonDocument::Compact);
                    udpServerSend->writeDatagram(datagram,QHostAddress(item.serverAddress), item.networkTcpPort.toInt());
                }
            }

            if(komutMesaji=="mouse-on") clientConfUpdate("mouseState",true);
            if(komutMesaji=="mouse-off") clientConfUpdate("mouseState",false);

            if(komutMesaji=="keyboard-on") clientConfUpdate("keyboardState",true);
            if(komutMesaji=="keyboard-off") clientConfUpdate("keyboardState",false);

            if(komutMesaji=="internet-on") clientConfUpdate("internetState",true);
            if(komutMesaji=="internet-off") clientConfUpdate("internetState",false);

            if(komutMesaji=="webblock-on") clientConfUpdate("webblockState",true);
            if(komutMesaji=="webblock-off") clientConfUpdate("webblockState",false);

            if(komutMesaji=="only-webblock-on") clientConfUpdate("onlywebblockState",true);
            if(komutMesaji=="only-webblock-off") clientConfUpdate("onlywebblockState",false);

            if(komutMesaji=="youtube-on") clientConfUpdate("youtubeState",true);
            if(komutMesaji=="youtube-off") clientConfUpdate("youtubeState",false);

        } else {
            qDebug() << "Hata çıktı:" << stdErr;
            QString hatamesaji=stdErr.left(255);
            // tray a gönder
            if(mesajVisible=="1")
            {
                QJsonObject sendTrayJson;
                //QString hatamesaji=stdErr.left(255);
                sendTrayJson["mainmessagetype"] = "consolecommand";
                sendTrayJson["submessagetype"] ="consolecommand" ;
                sendTrayJson["mission"] = command;
                sendTrayJson["missiondetail"] = stdErr;
                sendTrayJson["missionmessage"]=komutMesaji;
                sendTrayJson["messagevisible"]=mesajVisible;
                sendTrayJson["missionstate"] = "1";
                QByteArray datagram = QJsonDocument(sendTrayJson).toJson(QJsonDocument::Compact);
                udpTraySend->writeDatagram(datagram, QHostAddress::LocalHost, 51512);
            }
            //server a gönder
            for (const NetProfil &item : NetProfilList) {
                if (item.serverAddress=="") continue;
                if (item.selectedNetworkProfil==false) continue;
                if(item.networkBroadCastAddress!=""&&
                    item.serverAddress.section(".",0,2)==item.networkBroadCastAddress.section(".",0,2)&&
                    item.serverAddress.section(".",0,2)==item.ipAddress.section(".",0,2))
                {
                    // mainJson kopyasını al
                    QJsonObject sendServerJson;
                    // Network bilgilerini ekle
                    sendServerJson["command"] = command;
                    sendServerJson["command_detail"] = stdErr;
                    sendServerJson["command_status"] = "1";
                    sendServerJson["messagetype"]="commandstateclient";
                    sendServerJson["ip_address"] = item.ipAddress;
                    sendServerJson["mac_address"] = item.macAddress;
                    QByteArray datagram = QJsonDocument(sendServerJson).toJson(QJsonDocument::Compact);
                    udpServerSend->writeDatagram(datagram,QHostAddress(item.serverAddress), item.networkTcpPort.toInt());
                }
            }
         }

    }
    else {
        //treay a gönder
        if(mesajVisible=="1")
        {
            QString hatamesaji=stdErr.left(255);
            QJsonObject sendJson;
            sendJson["mainmessagetype"] = "consolecommand";
            sendJson["submessagetype"] ="consolecommand" ;
            sendJson["mission"] = command;
            sendJson["missiondetail"] = "Komut zamanında tamamlanamadı.";
            sendJson["missionmessage"]=komutMesaji;
            sendJson["messagevisible"]=mesajVisible;
            sendJson["missionstate"] = "1";
            QByteArray datagram = QJsonDocument(sendJson).toJson(QJsonDocument::Compact);
            udpTraySend->writeDatagram(datagram, QHostAddress::LocalHost, 51512);
        }
        //server a gönder
        for (const NetProfil &item : NetProfilList) {
            if (item.serverAddress=="") continue;
            if (item.selectedNetworkProfil==false) continue;
            if(item.networkBroadCastAddress!=""&&
                item.serverAddress.section(".",0,2)==item.networkBroadCastAddress.section(".",0,2)&&
                item.serverAddress.section(".",0,2)==item.ipAddress.section(".",0,2))
            {
                // mainJson kopyasını al
                QJsonObject sendServerJson;
                // Network bilgilerini ekle
                sendServerJson["command"] = command;
                sendServerJson["command_detail"] = "Komut zamanında tamamlanamadı.";
                sendServerJson["command_status"] = "1";
                sendServerJson["messagetype"]="commandstateclient";
                sendServerJson["ip_address"] = item.ipAddress;
                sendServerJson["mac_address"] = item.macAddress;
                QByteArray datagram = QJsonDocument(sendServerJson).toJson(QJsonDocument::Compact);
                udpServerSend->writeDatagram(datagram,QHostAddress(item.serverAddress), item.networkTcpPort.toInt());
            }
        }
     }

    qDebug() << "Komut Çalıştırıldı";
}



void Client::clientConfUpdate(QString field, bool state)
{
    DatabaseHelper *db=new DatabaseHelper(localDir+"clientConf.json");
    QJsonArray dizi=db->Oku();
    QJsonObject eskiVeri=QJsonObject();
    if(dizi.count()>0)
    {
        eskiVeri = dizi.first().toObject();
    }

    QJsonObject yeniVeri;
    yeniVeri[field] = state;

    mergeJson(eskiVeri, yeniVeri); // eskiVeri güncellendi!
    db->HepsiniSil();
    db->Ekle(eskiVeri);
}

void Client::clientConfLoad()
{
    DatabaseHelper *db=new DatabaseHelper(localDir+"clientConf.json");
    QJsonArray dizi=db->Oku();
    if(dizi.count()>0)
    {
        QJsonObject veri = dizi.first().toObject();
        mouseState = veri.value("mouseState").toBool(true);
        keyboardState = veri.value("keyboardState").toBool(true);
        internetState = veri.value("internetState").toBool(true);
        webblockState = veri.value("webblockState").toBool(true);
        onlywebblockState = veri.value("onlywebblockState").toBool(true);
        youtubeState = veri.value("youtubeState").toBool(true);
    }

clientConfWather.addPath(localDir+"clientConf.json");
}
void Client::webBlockAktifPasif(bool _state)
{
    webblockStateRun=true;
    if(_state)
    {
        //QString kmt28="iptables -F &";
        //system(kmt28.toStdString().c_str());
        //QString kmt29="ip6tables -F &";
        ///system(kmt29.toStdString().c_str());
        DatabaseHelper *db=new DatabaseHelper(localDir+"webblockserver.json");
        QJsonArray dizi=db->Ara("selectedWord",true);
        for (const QJsonValue &item : dizi) {
            QJsonObject veri=item.toObject();
            ///qDebug()<<veri["word"].toString();
            QString kmt28="/usr/bin/domain-block "+veri["word"].toString();
            //qDebug()<<"iptables komutu: "<<kmt28;
            system(kmt28.toStdString().c_str());
        }
    }
    if(!_state)
    {
       /* QString kmt28="iptables -F";
        system(kmt28.toStdString().c_str());
        QString kmt29="ip6tables -F";
        system(kmt29.toStdString().c_str());*/
        //QString kmt28="iptables -F &";
        //system(kmt28.toStdString().c_str());
        //QString kmt29="ip6tables -F &";
        //system(kmt29.toStdString().c_str());
        DatabaseHelper *db=new DatabaseHelper(localDir+"webblockserver.json");
        //QJsonArray dizi=db->Oku();
        QJsonArray dizi=db->Ara("selectedWord",true);
        for (const QJsonValue &item : dizi) {
            QJsonObject veri=item.toObject();
            QString kmt28="/usr/bin/domain-unblock "+veri["word"].toString();
            system(kmt28.toStdString().c_str());
        }
    }
}

QString Client::getIpPortStatus(QString service,int number)
{
    QString result="";
    QStringList arguments;
    arguments << "-c" << QString(service);
    QProcess process;
    process.start("/bin/bash",arguments);
    if(process.waitForFinished(-1))
    {
        result = process.readAll();
        result.chop(1);
    }
    // qDebug()<<"Port sorgulama:"<<result<<service;
    if(result.toInt()>number){ return "open";}
    else {return "close";}
}
bool Client::uygulamaCalisiyorMu(const QString& uygulamaAdi) {
    QString komut=uygulamaAdi;
    QStringList arguments;
    arguments << "-c" << komut;
    QProcess process;
    process.start("/bin/bash",arguments);
    process.waitForFinished();
    //qDebug()<<"çalışan komut: "<<komut;
    if (process.exitCode() == 0) {
        // Uygulama çalışıyor
        return true;
    } else {
        // Uygulama çalışmıyor
        return false;
    }
}
QString Client::findX11vncPort(QString kmt) {
    //qDebug()<<"x11vnc portu test ediliyor....."<<kmt;
    QString ports="";
    QProcess process;
    process.start(kmt);
    process.waitForFinished();
    QString output = process.readAllStandardOutput();
    //qDebug()<<"findX11vncPort test sonucu:"<<output.split("\n");

    QStringList lines = output.split("\n");
    for (const QString& line : lines) {


        if (line.contains("x11vnc")) {
            if (line.contains("tcp6")) {
                QStringList parts = line.split(" ");
                 //qDebug()<<"satir"<<line<<parts;
                for (const QString& part : parts) {
                    if (part.contains(":59")) {
                        //qDebug()<<"satir"<<part.split(":")[3];
                        //return part.split(":")[1];
                        ports=ports+part.split(":")[3]+"-";

                    }
                }
            }
        }
    }
    //qDebug()<<"findX11vncPort test sonucu00:"<<ports;


    return ports;
}
QString  Client::getSeatId()
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
QString Client::getSessionInfo(QString id, QString parametre)
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
Client::~Client()
{
    //QString data="portStatus|mydisp|noLogin|0|0|0|0|myenv|noLogin|0|0|0|0|0|0|0|close";
    //udpServerSendSlot(data);
    udpServerSend->close();
    udpServerSend->deleteLater();
}
