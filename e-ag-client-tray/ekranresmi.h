#ifndef EKRANRESMI_H
#define EKRANRESMI_H
#include<QDesktopWidget>
#include <QApplication>
#include<QtWidgets>
void MainWindow::kaydetTimerSlot(QString username,QString password)
{
  //  hostAddressMacButtonSlot();
      QString ip;
    /**********************host ve port okunuyor*****************************/
     FileCrud *fcc=new FileCrud();
     fcc->dosya="/usr/share/e-ag/hostport";
     if(fcc->fileExists())
     {
         QString hostport=fcc->fileGetLine(1);

         if (hostport!="")
         {
             ip=hostport.split("|")[0];
            // port=hostport.split("|")[1];
            // socket->connectToHost(ip,port.toInt());
         }
     }
     else qDebug()<<"hostport Dosyası Bulunamadı..";
/**************************************************/
     // qDebug()<<ip;

     Qt::AspectRatioMode aspectRatioMode = Qt::IgnoreAspectRatio;
     Qt::TransformationMode transformMode = Qt::FastTransformation;
     QPixmap desk = qApp->screens().at(0)->grabWindow(QDesktopWidget().winId());
     QPixmap px=desk.scaled(400,300,aspectRatioMode,transformMode);
    // hostAddressMacButtonSlot();//local ip adresi tspit ediliyor.

    /* for(int i=0;i<ipmaclist.count();i++)
     {
         QString ad=QDir::homePath()+"\/"+ipmaclist[i].ip+".png";
         qDebug()<<"dosya:"<<ad;
         px.save(ad);
         /*****************dosya server kopyalanıyor*********************/
       /*  QString komut="sshpass -p "+password+" scp -o StrictHostKeyChecking=no "+ad+" "+
                 username+"@"+ip+":\/tmp\/";
         // qDebug()<<komut;
         QStringList arguments;
         arguments << "-c" << komut;
         QProcess process;
         process.start("/bin/bash",arguments);
         process.waitForFinished(-1); // will wait forever until finished
         system("sleep 0.5");
     }*/

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
                         QString ad=QDir::homePath()+"\/"+ipmaclist[k].ip+".png";
                         qDebug()<<"dosya:"<<ad;
                         px.save(ad);
                         /*****************dosya server kopyalanıyor*********************/
                         QString komut="sshpass -p "+password+" scp -o StrictHostKeyChecking=no "+ad+" "+
                                 username+"@"+serverlist[0]+":\/tmp\/";
                         // qDebug()<<komut;
                         QStringList arguments;
                         arguments << "-c" << komut;
                         QProcess process;
                         process.start("/bin/bash",arguments);
                         process.waitForFinished(-1); // will wait forever until finished
                         system("sleep 0.5");
                        }
                 }
             }
         }
     }

}


#endif // EKRANRESMI_H
