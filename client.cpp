#include "client.h"
#include<QDebug>
#include<QProcess>
#include <QtCore/QTimer>
#include <stdio.h>
#include <QtCore/QCoreApplication>
#include <QtDBus/QtDBus>

Client::Client()
{
 //hostAddressMacButtonSlot();return;

    localDir="/usr/share/e-ag/";
    localDir1="/tmp/";

    QString kmt25="rm -rf "+localDir+"kilitclose";
    system(kmt25.toStdString().c_str());

    QString kmt26="rm -rf "+localDir+"kilitopen";
    system(kmt26.toStdString().c_str());


/************************************************/
    QTimer *tcpMesajSendTimer = new QTimer();
    QObject::connect(tcpMesajSendTimer, &QTimer::timeout, [&](){


tcpMesajSendTimerSlot();
    });
    tcpMesajSendTimer->start(3000);


    /***********************************/
    FileCrud *fcc=new FileCrud();
    fcc->dosya=localDir+"hostport";
    QStringList lst;
    if(!fcc->fileExists())
    {
        lst.append("|7879|Server");
        listToFile(localDir,lst,"hostport");
          system("chmod 777 /usr/share/e-ag/hostport");
    }
    QStringList hostfile=fileToList(localDir,"hostport");
    /**********************************************************/

    if(fcc->fileExists())
    {

        if (listGetLine(hostfile,"Server")!="")
        {
            QString hostportLine=listGetLine(hostfile,"Server");
            tcpPort=hostportLine.split("|")[1];
            /***************************************************/
            QString uport=tcpPort;
            std::reverse(uport.begin(), uport.end());
          //  QHostAddress *host  = new QHostAddress("192.168.63.254");
         //  QHostAddress *server = new QHostAddress("192.168.23.253");


           udpSocketSend = new QUdpSocket();
           udpSocketSendX11Mesaj= new QUdpSocket();
           udpSocketGet = new QUdpSocket();
           udpSocketGetMyDisp=new QUdpSocket();;
           udpSocketGetMyEnv=new QUdpSocket();;

           // udpSocketSend->connectToHost(*server,tcpPort.toInt());
          // udpSocketSend->bind(tcpPort.toInt(), QUdpSocket::ShareAddress);
            udpSocketGet->bind(uport.toInt(), QUdpSocket::ShareAddress);
            udpSocketGetMyDisp->bind(5555, QUdpSocket::ShareAddress);
            udpSocketGetMyEnv->bind(6666, QUdpSocket::ShareAddress);

           //udpSocketGet->bind(*host, uport.toInt());
            QObject::connect(udpSocketGet,&QUdpSocket::readyRead,[&](){udpSocketGetRead();});
            QObject::connect(udpSocketGetMyDisp,&QUdpSocket::readyRead,[&](){udpSocketGetMyDispRead();});
            QObject::connect(udpSocketGetMyEnv,&QUdpSocket::readyRead,[&](){udpSocketGetMyEnvRead();});

            qDebug()<<tcpPort<<uport<<"udp bağlandı";

        }


    }

    /**************************************/
    //connectHost();
}

void Client::tcpMesajSendTimerSlot()
{

   system("nohup /usr/bin/e-ag-run.sh&");

}

Client::~Client()
{

    udpSocketSend->close();
    udpSocketSend->deleteLater();


}


void Client::udpSocketSendTServer(QString _data)
{

    hostAddressMacButtonSlot();
    QStringList hostlist=fileToList("/usr/share/e-ag/","hostport");
    for(int i=0;i<hostlist.count();i++)
    {
        if(hostlist[i]!="")
        {
            QStringList serverlist=hostlist[i].split("|");
            if(serverlist[0]!="")
            {
                for(int k=0;k<ipmaclist.count();k++)
                {
                    if(serverlist[0].section(".", 0, 2)==ipmaclist[k].ip.section(".", 0,2))
                    {
                        QString msg="Client Mesaj|"+ipmaclist[k].ip+"|"+ipmaclist[k].mac+"|"+_data;
                        QByteArray datagram = msg.toUtf8();// +QHostAddress::LocalHost;
                        udpSocketSend->writeDatagram(datagram,QHostAddress(serverlist[0]), tcpPort.toInt());
                        qDebug()<<"server'a mesaj gönderildi:"<< serverlist[0]<<_data;
                    }
                }
            }
        }
    }
}
void Client::udpSocketGetMyDispRead()
{
    QByteArray datagram;
    QStringList mesaj;

    while (udpSocketGetMyDisp->hasPendingDatagrams()) {
        datagram.resize(int(udpSocketGetMyDisp->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;

        udpSocketGetMyDisp->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString rmesaj=datagram.constData();
     ///   qDebug()<<"x11'den Gelen Udp Mesaj:"<<rmesaj;
        x11env=rmesaj;

    }
}

void Client::udpSocketGetMyEnvRead()
{
    QByteArray datagram;
    QStringList mesaj;

    while (udpSocketGetMyEnv->hasPendingDatagrams()) {
        datagram.resize(int(udpSocketGetMyEnv->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;

        udpSocketGetMyEnv->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString rmesaj=datagram.constData();
     ///   qDebug()<<"e-ag-run'dan Gelen Udp Mesaj:"<<rmesaj;
        myenv=rmesaj;


        /******************************************************/
          if(x11env=="")
              x11env="noLogin|0|0|0|0";

         // else
          //    x11env=x11env;
         // QString result1=arg;
          bool sshState;
          int vncState;
          bool ftpState;
          /*************************************/
          if (getIpPortStatus("systemctl status ssh.service|grep 'running'|wc -l")=="open")
              sshState=true;
          else sshState=false;
          /*************************************/
          if (getIpPortStatus("systemctl status e-ag-x11vncdesktop.service|grep '5900'|wc -l")=="open")
              vncState=5900;
          else if (getIpPortStatus("systemctl status e-ag-x11vnclogin.service|grep '5902'|wc -l")=="open")
              vncState=5902;
          else vncState=0;
          /*************************************/
          if (getIpPortStatus("systemctl status vsftpd.service|grep 'running'|wc -l")=="open")
              ftpState=true;
          else ftpState=false;
          /*************************************/
          /// qDebug()<<"durum:"<<x11env<<result1;
          QString data="portStatus|mydisp|"+x11env+"|"+myenv+"|"+QString::number(sshState)+"|"+QString::number(vncState)+"|"+QString::number(ftpState);
          if(tempdata!=data)
          {
              udpSocketSendTServer(data);
              tempdata=data;
              dataSayac=0;
          }
          else dataSayac++;
          if(dataSayac>10)
          {
              udpSocketSendTServer(data);
              tempdata=data;
              dataSayac=0;
          }

          data="";
          x11env="";
          myenv="";


    }
}
void Client::udpSocketGetRead()
{
    QByteArray datagram;
    QStringList mesaj;

    while (udpSocketGet->hasPendingDatagrams()) {
        datagram.resize(int(udpSocketGet->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;

        udpSocketGet->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        QString rmesaj=datagram.constData();
        mesaj=rmesaj.split("|");

        ///  qDebug()<<"Gelen Udp Mesaj:"<<mesaj[0]<<mesaj[1]<<mesaj[2]<<mesaj[3];
        qDebug()<<"Gelen Udp Mesaj:"<<mesaj;
        QString x11mesaj="";
        bool sendStatus=true;
        if(mesaj[0]=="hostport")
        {
            //qDebug()<<"Gelen Udp Mesaj:"<<mesaj[2];
            QStringList hostlist=fileToList("/usr/share/e-ag/","hostport");
            if(hostlist.count()>2)hostlist.clear();

            hostlist=listRemove(hostlist,mesaj[2]);///mac ad. göre server kayıt
            hostlist.append(mesaj[1]+"|"+mesaj[3]+"|"+mesaj[2]+"|Server");

            listToFile(localDir,hostlist,"hostport");
            system("chmod 777 /usr/share/e-ag/hostport");
        }
        if(mesaj[0]=="tcpporttest")
        {
            /*  system("/usr/bin/e-ag-run.sh");
           QString result="";
           QStringList arguments;
           arguments << "-c" << QString("cat /tmp/mydisp");
           QProcess process;
           process.start("/bin/bash",arguments);
           if(process.waitForFinished())
           {
                result = process.readAll();
              //  qDebug()<<"session Bilgileri: "<<result;
                  result.chop(1);
           }
           QString result1="";
           QStringList arguments1;
           arguments1 << "-c" << QString("cat /tmp/myenv");
           QProcess process1;
           process1.start("/bin/bash",arguments1);
           if(process1.waitForFinished())
           {
                result1 = process1.readAll();
              //  qDebug()<<"session Bilgileri: "<<result;
                  result1.chop(1);
           }

          /*************************************/
            //if(socket->waitForConnected())
            ////   udpSocketSendTServer("tcpporttestopen|"+result+"|"+result1);
            //else udpSocketSendTServer("tcpporttestclose");
        }
        if(mesaj[0]=="sshporttest")
        {
            /*************************************/
            if (getIpPortStatus("systemctl status ssh.service|grep 'running'|wc -l")=="open")
                udpSocketSendTServer("sshporttestopen");
            else udpSocketSendTServer("sshporttestclose");
        }
        if(mesaj[0]=="vncporttest")
        {
            /*************************************/
            if (getIpPortStatus("systemctl status e-ag-x11vncdesktop.service|grep 'running'|wc -l")=="open"||
                    getIpPortStatus("systemctl status e-ag-x11vnclogin.service|grep 'running'|wc -l")=="open")
                udpSocketSendTServer("vncporttestopen");
            else udpSocketSendTServer("vncporttestclose");
        }
        if(mesaj[0]=="ftpporttest")
        {
            /*************************************/
            if (getIpPortStatus("systemctl status vsftpd.service|grep 'running'|wc -l")=="open")
                udpSocketSendTServer("ftpporttestopen");
            else udpSocketSendTServer("ftpporttestclose");
        }
        if(mesaj[0]=="kilitstatetrue")
        {
            QStringList liste;
            //  udpSocketSendTServer("kilitstatetestopen");
            x11mesaj=rmesaj; sendStatus=true;
        }
        if(mesaj[0]=="kilitstatefalse")
        {
            udpSocketSendTServer("kilitstatetestclose");
            x11mesaj=rmesaj; sendStatus=true;
        }
        if(mesaj[0]=="transparankilitstatetrue")
        {
            x11mesaj=rmesaj; sendStatus=true;
            //  udpSocketSendTServer("kilittransparanstatetestopen");
        }
        if(mesaj[0]=="transparankilitstatefalse")
        {
            x11mesaj=rmesaj; sendStatus=true;

            //  udpSocketSendTServer("kilittransparanstatetestopen");
        }
        if(mesaj[0]=="dosyatopla")
        {
            x11mesaj=rmesaj; sendStatus=true;
        }
        if(mesaj[0]=="ekranimagestatetrue")
        {
            x11mesaj=rmesaj; sendStatus=true;
            //      udpSocketSendTServer("ekranimagestatetestopen");
        }
        if(mesaj[0]=="ekranimagestatefalse")
        {
            x11mesaj=rmesaj; sendStatus=true;
            //      udpSocketSendTServer("ekranimagestatetestopen");
        }
        if(mesaj[0]=="webblock")
        {
            qDebug()<<"Client-Gelen Mesaj:"<<rmesaj;
            QStringList ayarlist=fileToList("/usr/share/e-ag/","e-ag-x11client.conf");
            //  ayarlist.append("kilitstate|"+QString::number(kilitstate));
            ayarlist=listRemove(ayarlist,"rootusername");
            ayarlist.append("rootusername|"+mesaj[6]);

            ayarlist=listRemove(ayarlist,"rootpassword");
            ayarlist.append("rootpassword|"+mesaj[7]);

            ayarlist=listRemove(ayarlist,"webblockstate");
            ayarlist.append("webblockstate|"+mesaj[1]);
            webblockstate=mesaj[1].toInt();

            QString kmt27="rm -rf /usr/share/e-ag/e-ag-x11client.conf";
            system(kmt27.toStdString().c_str());

            listToFile("/usr/share/e-ag",ayarlist,"e-ag-x11client.conf");
            QString kmt26="chmod 777 /usr/share/e-ag/e-ag-x11client.conf";
            system(kmt26.toStdString().c_str());
            /***********************************************/
            x11mesaj=rmesaj; sendStatus=true;
            /************************************************************************/

            webBlockAktifPasif();

        }
        if(mesaj[0]=="kilitstate")
        {
            qDebug()<<"Client-Gelen Mesaj:"<<rmesaj;
            QStringList ayarlist=fileToList("/usr/share/e-ag/","e-ag-x11client.conf");

            ayarlist=listRemove(ayarlist,"kilitstate");
            ayarlist.append("kilitstate|"+mesaj[1]);

            QString kmt27="rm -rf /usr/share/e-ag/e-ag-x11client.conf";
            system(kmt27.toStdString().c_str());

            listToFile("/usr/share/e-ag",ayarlist,"e-ag-x11client.conf");
            QString kmt26="chmod 777 /usr/share/e-ag/e-ag-x11client.conf";
            system(kmt26.toStdString().c_str());
            /***********************************************/
            x11mesaj=rmesaj; sendStatus=true;

            /************************************************************************/
        }
        if(mesaj[0]=="ekranmesaj")
        {
            x11mesaj=rmesaj; sendStatus=true;
        }
        if(mesaj[0]=="konsolexpect")
        {
            komutSudoExpect(mesaj[1],mesaj[6],mesaj[7]);

        }
        if(mesaj[0]=="x11komut")
        {
            // qDebug()<<"Client-Gelen Mesaj:"<<rmesaj;
            QString komut=mesaj[0]+"|"+mesaj[1]+"|"+mesaj[2]+"|"+mesaj[3]+"|"+mesaj[4]+"|"+mesaj[5]+"|"+mesaj[6]+"|"+mesaj[7];
            qDebug()<<"son hali:"<<komut;
            x11mesaj=komut; sendStatus=true;
        }
        if(mesaj[0]=="pckapat")
        {
            qDebug()<<"Client-Gelen Mesaj:"<<rmesaj;
            commandExecuteSlot(mesaj[1],mesaj[4],mesaj[5]);
        }
        if(mesaj[0]=="pcbaslat")
        {
            qDebug()<<"Client-Gelen Mesaj:"<<rmesaj;
            commandExecuteSlot(mesaj[1],mesaj[4],mesaj[5]);
        }
        if(sendStatus)
        {
            QByteArray datagram = x11mesaj.toUtf8();// +QHostAddress::LocalHost;
            udpSocketSendX11Mesaj->writeDatagram(datagram,QHostAddress::LocalHost, 5556);
            sendStatus=false;
            x11mesaj="";
        }

    }


}

void Client::webBlockAktifPasif()
{
    QStringList ayarlst=fileToList("/usr/share/e-ag/","e-ag-x11client.conf");
    /**********************************************************/
        if(listGetLine(ayarlst,"rootusername")!="")
        {
            QString strrootusername=listGetLine(ayarlst,"rootusername").split("|")[1];
            //qDebug()<<strwebblockstate;
            rootusername=strrootusername;
        }
        /**********************************************************/
            if(listGetLine(ayarlst,"rootpassword")!="")
            {
                QString strrootpassword=listGetLine(ayarlst,"rootpassword").split("|")[1];
                //qDebug()<<strwebblockstate;
                rootpassword=strrootpassword;
            }
/**********************************************************/
    if(listGetLine(ayarlst,"webblockstate")!="")
    {
        QString strwebblockstate=listGetLine(ayarlst,"webblockstate").split("|")[1];
        //qDebug()<<strwebblockstate;
        webblockstate=strwebblockstate.toInt();
    }
    /******************************************************************/
       if(webblockstate==true)
    {


         QString kmt28="/usr/share/e-ag/webdisable.sh";
         system(kmt28.toStdString().c_str());

    }
    if(webblockstate==false)
    {
        QString kmt28="iptables -F";
        system(kmt28.toStdString().c_str());

    }
    /*********************************************************/

}

void Client::komutSudoExpect(QString komut,QString username,QString password)
{
    qDebug()<<"komutexecute:"<<komut<<username<<password;

    QStringList liste;
    liste<<"#!/usr/bin/expect";
    liste<<"spawn su - "+username;
    liste<<"expect \"Parola:\"";
    liste<<"send \""+password+"\\n\"";
    liste<<"send \""+password+"\\n\"";
    liste<<"send \"echo "+password+"|sudo -S bash -e \""+komut+"\"\\n\"";
    liste<<"send \"exit\\n\"";
    liste<<"interact";
   // qDebug()<<"a0";
    listToFile("/home/"+username+"/",liste,"run.sh");
//qDebug()<<"a1";
    QString kmt22="chmod 777 /home/"+username+"/run.sh";
  // qDebug()<<"a11";
    system(kmt22.toStdString().c_str());

//qDebug()<<"a2";
    QString result="";
    QStringList arguments;
    arguments << "-c" << QString("/home/"+username+"/run.sh");
    QProcess process;
    process.start("/bin/bash",arguments);
    if(process.waitForFinished())
    {
        // version = process.readAll();
        //   result.chop(3);
    }

    QString kmt24="rm -rf /home/"+username+"/run.sh";
    system(kmt24.toStdString().c_str());
//qDebug()<<"finalll";
}

QString Client::getIpPortStatus(QString service)
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
    qDebug()<<"Port sorgulama:"<<result<<service;
    if(result.toInt()>0){ return "open";}
    else {return "close";}
}
/*QString Client::getIpPortStatus(QString ip_,QString port)
{
    QString result="";
    QStringList arguments;
    arguments << "-c" << QString("nc -zv -w1 %1 %2 2>&1|grep 'open'|wc -l").arg(ip_,port);
    QProcess process;
    process.start("/bin/bash",arguments);
    if(process.waitForFinished(-1))
    {
        result = process.readAll();
        result.chop(1);
    }
   // qDebug()<<"Port sorgulama:"<<ip_<<result<<port;
    if(result.toInt()>0){ return "open";}
    else {return "close";}
}*/



QStringList Client::listRemove(QStringList list,QString data)
 {
       QRegularExpression re(data);
     for(int i=0;i<list.count();i++)if(list[i].contains(re)) list.removeAt(i);
    // qDebug()<<list;
     return list;
 }
QString Client::listGetLine(QStringList list,QString data)
 {
     //QStringList list;
      QRegularExpression re(data);
     for(int i=0;i<list.count();i++) if(list[i].contains(re)) return list[i];
     //qDebug()<<list;
     return "";
 }
QStringList Client::fileToList(QString path,QString filename)
 {
    FileCrud *fcc=new FileCrud();
    fcc->dosya=path+filename;
    QStringList list;
    for(int i=1;i<=fcc->fileCount();i++)
    {
         QString line=fcc->fileGetLine(i);
         if(line!="")
         {
             line.chop(1);
             QStringList lst=line.split("|");
             QString ln="";
             if(lst.count()>0)ln.append(lst[0]);
             if(lst.count()>1)ln.append("|").append(lst[1]);
             if(lst.count()>2)ln.append("|").append(lst[2]);
             if(lst.count()>3)ln.append("|").append(lst[3]);
             if(lst.count()>4)ln.append("|").append(lst[4]);
             if(lst.count()>5)ln.append("|").append(lst[5]);
             if(lst.count()>6)ln.append("|").append(lst[6]);
             if(lst.count()>7)ln.append("|").append(lst[7]);
             if(lst.count()>8)ln.append("|").append(lst[8]);
             if(lst.count()>9)ln.append("|").append(lst[9]);
             if(lst.count()>10)ln.append("|").append(lst[10]);
             if(lst.count()>11)ln.append("|").append(lst[11]);
             if(lst.count()>12)ln.append("|").append(lst[12]);
             if(lst.count()>13)ln.append("|").append(lst[13]);

             list <<ln;
            // qDebug()<<ln;
             // list <<lst[0]+"|"+lst[1]+"|"+lst[2]+"|"+lst[3]+"|"+lst[4]+"|"+lst[5];

         }
    }
       return list;
 }
void Client::listToFile(QString path,QStringList list, QString filename)
 {
    FileCrud *fcc=new FileCrud();
    fcc->dosya=path+filename;
    fcc->fileRemove();
    for(int i=0;i<list.count();i++)
    {
         QString line=list[i];
         if(line!="")
         {
             //line.chop(1);
             QStringList lst=line.split("|");
             //qDebug()<<line;
             QString ln="";

             if(lst.count()>0)ln.append(lst[0]);
             if(lst.count()>1)ln.append("|").append(lst[1]);
             if(lst.count()>2)ln.append("|").append(lst[2]);
             if(lst.count()>3)ln.append("|").append(lst[3]);
             if(lst.count()>4)ln.append("|").append(lst[4]);
             if(lst.count()>5)ln.append("|").append(lst[5]);
             if(lst.count()>6)ln.append("|").append(lst[6]);
             if(lst.count()>7)ln.append("|").append(lst[7]);
             if(lst.count()>8)ln.append("|").append(lst[8]);
             if(lst.count()>9)ln.append("|").append(lst[9]);
             if(lst.count()>10)ln.append("|").append(lst[10]);
             if(lst.count()>11)ln.append("|").append(lst[11]);
             if(lst.count()>12)ln.append("|").append(lst[12]);
             if(lst.count()>13)ln.append("|").append(lst[13]);
             //qDebug()<<ln;
             fcc->fileWrite(ln);

         }

    }
 }

void Client::hostAddressMacButtonSlot()
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
                  // qDebug()<<"broadcast  address:"<<entry.broadcast().toString();
               ///  qDebug()<<"type:"<<networkInterface.name()<<networkInterface.type();
                 if(networkInterface.type()==QNetworkInterface::Ethernet)
                 {
                   QString kmt27="ethtool -s "+networkInterface.name()+" wol g";
                  system(kmt27.toStdString().c_str());
                 }
               }
           }
       }


}

void Client::commandExecuteSlot(QString command,QString username,QString password)
{
    //sshpass -p $parola ssh -o StrictHostKeyChecking=no -n $username@$name "echo $parola | sudo -S" $komut
///QString komut="echo '"+password+"'|sudo -S -u "+username+" -i "+command;
//echo '12' |sudo -S -u root -i apt-get update'
QString komut=command;


//mesajSlot(komut);
qDebug()<<komut;
QStringList arguments;
//QString  komut;
//komut.append("vncviewer ").append(pcIp->text()).append(" \-passwd \/usr\/bin\/x11vncpasswd");
arguments << "-c" << komut;

process.start("/bin/bash",arguments);
process.waitForFinished(-1); // will wait forever until finished
QString stdout = process.readAllStandardOutput();
QString stderr = process.readAllStandardError();
stdout.chop(1);
stderr.chop(1);
 qDebug()<<"out:"<<stdout<<stdout.count();
  qDebug()<<"err:"<<stderr<<stderr.count();
 //if(stdout)
//this->ui->lineEdit_message->setText(stdout);
//on_pushButton_sendMessage_clicked();
//this->ui->lineEdit_message->setText(stderr);
//on_pushButton_sendMessage_clicked();

//mesajSlot("Komut Çalıştırıldı");
qDebug()<<"Komut Çalıştırıldı";
}
