#include "networkprofil.h"
#include<QDebug>
#include<QProcess>
#include <QtCore/QTimer>
#include <stdio.h>
#include <QtCore/QCoreApplication>
#include<Database.h>

NewtworkProfil::NewtworkProfil()
{
    localDir="/usr/share/e-ag/";
    localDir1="/tmp/";

    networkProfilLoad();
    QString uport="7879";
    if(NetProfilList.count()>0)
        uport=NetProfilList.first().networkTcpPort;
    std::reverse(uport.begin(), uport.end());
    udpServerGet = new QUdpSocket();
    ////udpServerGet->bind(uport.toInt()+uport.toInt(), QUdpSocket::ShareAddress);
    udpServerGet->bind(QHostAddress::AnyIPv4, 45454,
                    QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    udpServerGet->joinMulticastGroup(QHostAddress("239.255.0.11"));
    QObject::connect(udpServerGet,&QUdpSocket::readyRead,[&](){udpServerGetSlot(); });

}


NewtworkProfil::~NewtworkProfil()
{
    //QString data="portStatus|mydisp|noLogin|0|0|0|0|myenv|noLogin|0|0|0|0|0|0|0|close";
    udpServerGet->close();
    udpServerGet->deleteLater();
}
void NewtworkProfil::udpServerGetSlot()
{
    QByteArray datagram;
    QStringList mesaj;
    while (udpServerGet->hasPendingDatagrams()) {
        datagram.resize(int(udpServerGet->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udpServerGet->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QString rmesaj=datagram.constData();
        mesaj=rmesaj.split("|");
        //qDebug()<<"Server Mesaj:"<<mesaj;
        if(mesaj[0]=="eagconf")
        {
            qDebug()<<"Server Get Messaje"<<mesaj;
            NetProfil np;
            np.networkIndex="";
            np.selectedNetworkProfil=true;
            np.networkName="network";
            np.serverAddress=mesaj[1];
            np.ipAddress="";
            np.macAddress="";
            np.networkBroadCastAddress=mesaj[2];
            np.networkTcpPort=mesaj[3];
            np.ftpPort=mesaj[4];
            np.rootPath=mesaj[5];
            np.language=mesaj[6];
            np.lockScreenState=stringToBool(mesaj[7]);
            np.webblockState=stringToBool(mesaj[8]);
            bool findStatus=false;
            for(int i=0;i<NetProfilList.count();i++)
            {
                qDebug()<<"NetProfilList:"<<i<<NetProfilList[i].networkBroadCastAddress;
                if(np.networkBroadCastAddress==NetProfilList[i].networkBroadCastAddress)
                {

                    findStatus=true;
                    bool updateState=false;
                    np.ipAddress=NetProfilList[i].ipAddress;
                    for(int k=0;k<interfaceList.count();k++)
                    {
                        if(NetProfilList[i].ipAddress.section(".",0,2)==interfaceList[k].ip.section(".",0,2))
                        {
                            np.ipAddress=interfaceList[k].ip;
                        }
                    }
                    np.ipAddress=NetProfilList[i].ipAddress;
                    np.macAddress=NetProfilList[i].macAddress;
                    if(NetProfilList[i].serverAddress!=np.serverAddress)updateState=true;
                    if(NetProfilList[i].networkBroadCastAddress!=np.networkBroadCastAddress) updateState=true;
                    if(NetProfilList[i].networkTcpPort!=np.networkTcpPort) updateState=true;
                    if(NetProfilList[i].ftpPort!=np.ftpPort) updateState=true;
                    if(NetProfilList[i].rootPath!=np.rootPath) updateState=true;
                    if(NetProfilList[i].language!=np.language) updateState=true;
                    if(NetProfilList[i].lockScreenState!=np.lockScreenState) updateState=true;
                    if(NetProfilList[i].webblockState!=np.webblockState) updateState=true;
                    if(updateState)
                    {
                        qDebug()<<"eagconf bilgileri farklı güncelleniyor.";
                        networkProfilSave(np);
                        system("systemctl restart e-ag-client-console.service");
                    }
                    /*else
                    {
                        qDebug()<<"eagconf bilgileri aynı...";
                    }*/
                }
            }
            if(!findStatus)
            {
                qDebug()<<"eagconf bilgileri yok ekleniyor...";
                networkProfilSave(np);
                system("systemctl restart e-ag-client-console.service");
            }

        }
    }
}

void NewtworkProfil::networkProfilSave(NetProfil np)
{
   // qDebug()<<"server get data:"<<data;
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
    veri["ftpPort"]=np.ftpPort;
    veri["rootPath"]=np.rootPath;
    veri["language"]=np.language;
    veri["lockScreenState"]=np.lockScreenState;
    veri["webblockState"]=np.webblockState;
    db->Sil("networkName","networknullip");//ilk ip olmayan eklenen kayıt silinir
    db->Sil("networkBroadCastAddress",np.networkBroadCastAddress);
    db->Ekle(veri);
    networkProfilLoad();
}
void NewtworkProfil::networkProfilLoad()
{

    qDebug()<<"networkProfilLoad: "<<NetProfilList.count()
             <<"interfaceList: "<<interfaceList.count();
    hostAddressMacButtonSlot();
    //if(interfaceList.count()==0)  return;
    DatabaseHelper *db=new DatabaseHelper(localDir+"e-ag.json");
    //QJsonArray dizi=db->Oku();
    QJsonArray dizi=db->Ara("selectedNetworkProfil",true);

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
            np.networkBroadCastAddress=veri["networkBroadCastAddress"].toString();
            np.serverAddress=veri["serverAddress"].toString();
            hostAddressMacButtonSlot();
            for(int i=0;i<interfaceList.count();i++)
            {
                if(veri["ipAddress"].toString().section(".",0,2)==interfaceList[i].ip.section(".",0,2))
                {
                    np.ipAddress=interfaceList[i].ip;
                }
            }
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
            veri["ftpPort"]="12345";
            veri["rootPath"]="/tmp/";
            veri["language"]="tr_TR";
            veri["lockScreenState"]=false;
            veri["webblockState"]=false;
            db->Ekle(veri);
        }
        networkProfilLoad();
        qDebug()<<"eagconf bilgileri farklı güncelleniyor.";
        system("systemctl restart e-ag-client-console.service");
    }
}
void NewtworkProfil::hostAddressMacButtonSlot()
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
                  /*
                 if(networkInterface.type()==QNetworkInterface::Ethernet)
                 {

                     //QString program = "notepad.exe"; // Çalıştırılacak uygulamanın yolu (Windows için)
                     QStringList arguments;
                     arguments << "&"; // Uygulamaya geçirilecek argümanlar (isteğe bağlı)

                     QProcess *process = new QProcess();

                     // Uygulamanın başlamasıyla ilgili bir sinyali bağlayabiliriz (isteğe bağlı)
                     QObject::connect(process, &QProcess::started, [=](){
                         qDebug() << "Uygulama başlatıldı:" << program;
                     });

                     // Uygulamanın bitmesiyle ilgili bir sinyali bağlayabiliriz (isteğe bağlı)
                     QObject::connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                                      [=](int exitCode, QProcess::ExitStatus exitStatus){
                                          qDebug() << "Uygulama bitti. Çıkış kodu:" << exitCode
                                                   << ", Durum:" << (exitStatus == QProcess::NormalExit ? "Normal" : "Çökme");
                                          process->deleteLater(); // İşlem nesnesini sil
                                      });

                     // Uygulamayı asenkron olarak başlat
                     process->start(program, arguments);

                     if (!process->waitForStarted(1000)) { // Uygulamanın başlayıp başlamadığını kısa bir süre kontrol et (isteğe bağlı)
                         qDebug() << "Uygulama başlatılamadı:" << process->errorString();
                         process->deleteLater();
                     } else {
                         qDebug() << "Uygulama başlatılıyor ve ana akış devam ediyor...";
                         // Burada ana uygulamanızın diğer işlemleri devam edebilir.
                     }

                  //system(kmt27.toStdString().c_str());

                 }*/
               }
           }
       }


}
