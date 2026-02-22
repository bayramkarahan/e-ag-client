#include "networkprofil.h"
#include<QDebug>
#include<QProcess>
#include <QtCore/QTimer>
#include <stdio.h>
#include <QtCore/QCoreApplication>
#include<Database.h>

NewtworkProfil::NewtworkProfil()
{
    networkProfilSaveStatus=false;
    localDir="/usr/share/e-ag/";
    localDir1="/tmp/";
   networkProfilWather.addPath(localDir+"e-ag.json");
    connect(&networkProfilWather, &QFileSystemWatcher::fileChanged, this,
            [this](){
                if(!networkProfilSaveStatus)
                {
                qDebug()<<"Ayarlar güncellendi(Farklı Uygulamalardan)...";
                networkProfilLoad();  // burada tekrar addPath() çağırılacak
                }
            });
    networkProfilLoad();
    QString uport="7879";
    if(NetProfilList.count()>0)
        uport=NetProfilList.first().networkTcpPort;
    std::reverse(uport.begin(), uport.end());
    udpServerGet = new QUdpSocket();
    ////udpServerGet->bind(uport.toInt()+uport.toInt(), QUdpSocket::ShareAddress);
    udpServerGet->bind(QHostAddress::AnyIPv4, 45454,
                    QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    QHostAddress group("239.255.0.11");
    //udpServerGet->joinMulticastGroup(QHostAddress("239.255.0.11"));
    /*********************************************/
    const QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &iface : interfaces)
    {
        if (iface.flags().testFlag(QNetworkInterface::IsUp) &&
            iface.flags().testFlag(QNetworkInterface::IsRunning) &&
            iface.flags().testFlag(QNetworkInterface::CanMulticast) &&
            !iface.humanReadableName().contains("lo"))
        {
            bool ok = udpServerGet->joinMulticastGroup(group, iface);
            qDebug() << "Join:" << iface.name() << ok;
        }
    }
    /**********************************************/
    QObject::connect(udpServerGet,&QUdpSocket::readyRead,[&](){udpServerGetSlot(); });

/*********************kullanılmayan profilleri sil******************************/

}


NewtworkProfil::~NewtworkProfil()
{
    //QString data="portStatus|mydisp|noLogin|0|0|0|0|myenv|noLogin|0|0|0|0|0|0|0|close";
    udpServerGet->close();
    udpServerGet->deleteLater();
}
void NewtworkProfil::udpServerGetSlot()
{
    //qDebug()<<"Server Get Message........."<<networkProfilSaveStatus;
    if (networkProfilSaveStatus)return;

    QJsonObject getJson;
    QByteArray datagram;
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
            //qDebug()<<"Server Get Message-0:"<<getJson;
        } else {
            qWarning() << "Hatalı Mesaj:" << datagram.constData();
            qWarning() << "Tray JSON parse hatası:" << parseError.errorString();
            return;
        }
        /***************************************/


        QString mainmessagetype = getJson["mainmessagetype"].toString();
        if(mainmessagetype=="eagconf")
        {
            //qDebug()<<"Server Get Message-1:"<<getJson;
            NetProfil np;
            np.networkIndex="";
            np.selectedNetworkProfil=true;
            np.networkName="network";
            np.serverAddress=getJson["server_address"].toString();
            np.ipAddress="";
            np.macAddress="";
            np.networkBroadCastAddress=getJson["networkBroadCastAddress"].toString();
            for(int i=0;i<interfaceList.count();i++)
            {
                if (ayniSubnet(np.serverAddress,
                               interfaceList[i].ip,
                               interfaceList[i].subnet))
                {
                    np.ipAddress = interfaceList[i].ip;
                    np.macAddress = interfaceList[i].mac;
                    np.subnet = interfaceList[i].subnet;
                    np.networkBroadCastAddress = interfaceList[i].broadcast;
                    break;
                }
            }
            np.networkTcpPort=getJson["networkTcpPort"].toString();
            np.ftpPort=getJson["ftpPort"].toString();
            np.rootPath=getJson["rootPath"].toString();
            np.language=getJson["language"].toString();
            np.lockScreenState=getJson["lockScreenState"].toBool();
            np.webblockState=getJson["webblockState"].toBool();
            bool findStatus=false;
            for(int i=0;i<NetProfilList.count();i++)
            {
                if(np.networkBroadCastAddress==NetProfilList[i].networkBroadCastAddress)
                {
                    if(np!=NetProfilList[i])
                    {
                        findStatus=true;
                        qDebug()<<"ağ kayıdı var ekleniyor"<<np.networkBroadCastAddress<<NetProfilList[i].networkBroadCastAddress;
                        qDebug()<<"NetProfilList Farklı: Gelen"<<getJson;
                        networkProfilSave(np);
                    }
                //}

            }
            }
            /*********ilk defa olacaksa ***************/
            bool newfindStatus=false;
            if(!findStatus)
            {
                for(int i=0;i<NetProfilList.count();i++)
                {
                    //qDebug()<<"np:"<<np.networkBroadCastAddress.section(".",0,1)
                    //         <<"npl:"<<NetProfilList[i].networkBroadCastAddress.section(".",0,1);
                    if(np.networkBroadCastAddress==NetProfilList[i].networkBroadCastAddress)
                    {
                        newfindStatus=true; //kayıt var
                    }
                }
                if(!newfindStatus)
                {
                qDebug()<<"ağ kayıdı yok ekleniyor"<<np.ipAddress;
                qDebug()<<"Yeni NetProfilList:"<<getJson;
                networkProfilSave(np);
                newfindStatus=false;
                }
            }

        }
    }


}
void NewtworkProfil::networkProfilSave(NetProfil np)
{
    networkProfilSaveStatus=true;
    qDebug()<<"networkProfilSave:"<<np.serverAddress<<np.ipAddress;
    DatabaseHelper *db=new DatabaseHelper(localDir+"e-ag.json");
    QJsonObject veri;
    veri["networkIndex"]= QString::number(db->getIndex("networkIndex"));
    veri["selectedNetworkProfil"] =np.selectedNetworkProfil;
    veri["networkName"] =np.networkName;
    veri["networkTcpPort"] = np.networkTcpPort;
    veri["serverAddress"]=np.serverAddress;
    veri["ipAddress"]=np.ipAddress;
    veri["macAddress"]=np.macAddress;
    veri["networkBroadCastAddress"]=np.networkBroadCastAddress;
    veri["subnet"]=np.subnet;
    veri["ftpPort"]=np.ftpPort;
    veri["rootPath"]=np.rootPath;
    veri["language"]=np.language;
    veri["lockScreenState"]=np.lockScreenState;
    veri["webblockState"]=np.webblockState;
    db->Sil("networkBroadCastAddress",np.networkBroadCastAddress);
    db->Ekle(veri);
    db->Sil("networkName","networknullip");//ilk ip olmayan eklenen kayıt silinir
    //db->Sil("serverAddress",np.serverAddress);
    db->Sil("macAddress","99999");
    db->Sil("macAddress","3333");
    db->Sil("serverAddress","");
    networkProfilSaveStatus=false;
    //işlem bitin load yapılıyor...
    networkProfilLoad();
}
void NewtworkProfil::networkProfilLoad()
{

    qDebug()<<"networkProfilLoad: "<<NetProfilList.count()
             <<"interfaceList: "<<interfaceList.count()
             <<"networkProfilSaveStatus: "<<networkProfilSaveStatus;
    hostAddressMacButtonSlot();
    DatabaseHelper *db=new DatabaseHelper(localDir+"e-ag.json");
    QJsonArray dizi=db->Oku();
    //QJsonArray dizi=db->Ara("selectedNetworkProfil",true);

    if(dizi.count()>0)
    {
        NetProfilList.clear();
        for (const QJsonValue &item : dizi) {
            QJsonObject veri=item.toObject();
            //qDebug()<<"Yüklenen Ağ Profili:" <<veri;
            NetProfil np;
            np.networkIndex=veri["networkIndex"].toString();
            np.selectedNetworkProfil=veri["selectedNetworkProfil"].toBool();
            np.networkName=veri["networkName"].toString();
            np.networkTcpPort=veri["networkTcpPort"].toString();
            np.serverAddress=veri["serverAddress"].toString();
            hostAddressMacButtonSlot();
            for(int i=0;i<interfaceList.count();i++)
            {
                if (ayniSubnet(veri["ipAddress"].toString(),
                               interfaceList[i].ip,
                               interfaceList[i].subnet))
                {
                    np.ipAddress = interfaceList[i].ip;
                    np.macAddress = interfaceList[i].mac;
                    np.subnet = interfaceList[i].subnet;
                    np.networkBroadCastAddress = interfaceList[i].broadcast;
                }
               /* if (ayniSubnet(np.serverAddress,
                               NetProfilList[i].serverAddress,
                               np.subnet))
                {
                    np.ipAddress=interfaceList[i].ip;
                    np.macAddress=interfaceList[i].mac;
                    np.subnet=interfaceList[i].subnet;
                    np.networkBroadCastAddress=veri["networkBroadCastAddress"].toString();
                }*/
               /* if(veri["ipAddress"].toString().section(".",0,1)==interfaceList[i].ip.section(".",0,1))
                {
                    np.ipAddress=interfaceList[i].ip;
                    np.macAddress=interfaceList[i].mac;
                    np.subnet=interfaceList[i].subnet;
                    np.networkBroadCastAddress=veri["networkBroadCastAddress"].toString();
                }*/
            }

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
            //qDebug()<<"broadcast address:"<<i<<interfaceList[i].broadcast;
            QJsonObject veri;
            veri["networkIndex"] =QString::number(db->getIndex("networkIndex"));
            veri["selectedNetworkProfil"] =true;
            veri["networkName"] = "network";
            veri["networkTcpPort"] = "7879";
            veri["serverAddress"]="";
            veri["ipAddress"]=interfaceList[i].ip;
            veri["macAddress"]=interfaceList[i].mac;
            veri["networkBroadCastAddress"]=interfaceList[i].broadcast;
            veri["subnet"]=interfaceList[i].subnet;

            veri["ftpPort"]="12345";
            veri["rootPath"]="/tmp/";
            veri["language"]="tr_TR";
            veri["lockScreenState"]=false;
            veri["webblockState"]=false;
            if(interfaceList[i].ip.contains("172.17"))veri["selectedNetworkProfil"] =false;
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
            veri["serverAddress"]="11111";
            veri["ipAddress"]="2222";
            veri["macAddress"]="3333";
            veri["networkBroadCastAddress"]="";
            veri["subnet"]="";
            veri["ftpPort"]="12345";
            veri["rootPath"]="/tmp/";
            veri["language"]="tr_TR";
            veri["lockScreenState"]=false;
            veri["webblockState"]=false;
            db->Ekle(veri);
        }
        networkProfilLoad();

    }
    //qDebug()<<"eagconf bilgileri farklı güncelleniyor.";
    ///system("systemctl restart e-ag-client-console.service");
}
void NewtworkProfil::hostAddressMacButtonSlot()
{
    QHostAddress localhost = QHostAddress(QHostAddress::LocalHost);
    interfaceList.clear();
    foreach (const QNetworkInterface& networkInterface, QNetworkInterface::allInterfaces()) {
           foreach (const QNetworkAddressEntry& entry, networkInterface.addressEntries()) {
               //QHostAddress *hostadres=new QHostAddress(entry.ip().toString());
               QHostAddress hostadres(entry.ip());

               if(hostadres.protocol() == QAbstractSocket::IPv4Protocol &&
                       !hostadres.isLoopback() )
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
