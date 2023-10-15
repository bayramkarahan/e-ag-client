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
#include<QTabWidget>
#include<QLabel>
#include<QGridLayout>
#include<QPushButton>
#include<QFile>
#include<QFileDialog>
#include<QRegularExpression>
#include<filecrud.h>
#include<QTimeEdit>
#include<QDate>
#include<QTimer>
#include<QTime>
#include<QMessageBox>
#include<ayar.h>
#include<giris.h>
#include<hakkinda.h>
#include<QApplication>
#include<QDesktopWidget>
#include<QMenu>
#include<QCloseEvent>
#include<QProcess>
#include<QAuthenticator>
#include<QApplication>
#include <stdio.h>
#include <QtCore/QCoreApplication>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
 {
    /*******************************************************************/
    webblockcb= new QCheckBox("Her Açılışta Web Sitelerini Engelle.");
    twl=new QTableWidget;
    gelenKomut=new QLabel("-------------------");
    localDir="/tmp/";

    this->resize(340,300);
    setFixedWidth(400);
    setFixedHeight(400);
    setWindowTitle("e-ag-client-gui");
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()/2 - this->width()/2);
    int y = (screenGeometry.height() - this->height()) / 2;
    this->move(x, y);
    this->setStyleSheet("background-color: #dfdfdf;");


       // App icon
      auto appIcon = QIcon(":/icons/e-ag-client.svg");
      this->setWindowIcon(appIcon);

      aw=ayar();

      //aw->setEnabled(false);
       /************************************************************/
        tw=new QTabWidget(this);
        tw->resize(this->width(),this->height());
        tw->addTab(giris(),"Giriş");
        tw->addTab(aw,"Ayarlar");
        tw->addTab(hakkinda(),"Hakkında");


/***************************************************************/
        webBlockAktifPasif();
/***************************************************************/
    QStringList arguments;
        arguments << "-c" << QString("printenv USER");
        QProcess process;
        process.start("/bin/bash",arguments);
        if(process.waitForFinished())
        {
            x11user = process.readAll();
           /// qDebug()<<"mydisp user bilgi:"<<user;
               x11user.chop(1);
        }
        QStringList argumentss;
        argumentss << "-c" << QString("printenv DISPLAY");


        process.start("/bin/bash",argumentss);
        if(process.waitForFinished())
        {
            x11display = process.readAll();
            ///qDebug()<<"mydisp display bilgi:"<<display;
               x11display.chop(1);
        }


        if(!x11display.contains("0", Qt::CaseInsensitive))//!=0
        {
             QString kmt20="nohup /usr/bin/x11vnc -forever -loop -noxdamage -repeat -rfbauth /usr/bin/x11vncpasswd -rfbport 5901 -shared &";
             system(kmt20.toStdString().c_str());
        }


       }


void MainWindow::webBlockAktifPasif()
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
        webblockcb->setChecked(webblockstate);
    }
    /******************************************************************/
    if(webblockstate==true)
    {
     komutSudoExpect("/usr/share/e-ag/webdisable.sh",rootusername,rootpassword);
    }
    if(webblockstate==false)
    {
    komutSudoExpect("iptables -F",rootusername,rootpassword);
    }
       /*********************************************************/

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
  //  delete ui;
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

QStringList MainWindow::listRemove(QStringList list,QString data)
 {
       QRegularExpression re(data);
     for(int i=0;i<list.count();i++)if(list[i].contains(re)) list.removeAt(i);
    // qDebug()<<list;
     return list;
 }
QString MainWindow::listGetLine(QStringList list,QString data)
{
    QRegularExpression re(data);
   for(int i=0;i<list.count();i++) if(list[i].contains(re)) return list[i];
   //qDebug()<<list;
   return "";
}
QStringList MainWindow::fileToList(QString path, QString filename)
{
  FileCrud *fcc=new FileCrud();
  fcc->dosya=path+filename;
 // qDebug()<<fcc->dosya;
  QStringList list;
  for(int i=1;i<=fcc->fileCount();i++)
  {
       QString line=fcc->fileGetLine(i);
      // qDebug()<<line;
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
           //qDebug()<<ln;
           // list <<lst[0]+"|"+lst[1]+"|"+lst[2]+"|"+lst[3]+"|"+lst[4]+"|"+lst[5];

       }
  }
     return list;
}
void MainWindow::listToFile(QString path,QStringList list, QString filename)
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
