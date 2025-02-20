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
#include<QCheckBox>
#include<QProcess>
#include<QProgressDialog>
#include<QToolButton>

#ifndef AYAR_H
#define AYAR_H

QWidget *MainWindow::ayar()
{

    QWidget *ayarPage=new QWidget();
     QFont f2( "Arial", 8, QFont::Normal);
    /*******************************************************/
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
    /*******************************************************/


    QLineEdit *webadres=new QLineEdit;
    webadres->setFixedSize(boy*9,boy);
    webadres->setText(user);
    webadres->setStyleSheet("background-color: #ffffff");

     /********************************************************/
    QToolButton *webadresEkleButton= new QToolButton;
    webadresEkleButton->setFixedSize(QSize(boy*4,boy));
    webadresEkleButton->setIconSize(QSize(boy,boy));
    webadresEkleButton->setStyleSheet("Text-align:center");
  //  webadresEkleButton->setIcon(QIcon(":/icons/save.svg"));
    webadresEkleButton->setAutoRaise(true);
    webadresEkleButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    webadresEkleButton->setFont(f2);
    webadresEkleButton->setText("Web Listesine Ekle");

    connect(webadresEkleButton, &QPushButton::clicked, [=]() {
       /* kontrol();
        if (status==false) return;
          /************************************************************************/
          QStringList webblocklist=fileToList("/usr/share/e-ag/","webblocklist");

              webblocklist<<webadres->text();

          listToFile("/usr/share/e-ag/",webblocklist,"webblocklist");
          for(int i=0;i<webblocklist.count();i++)
          {
              QString line=webblocklist[i];
              QStringList lst=line.split("|");
              twl->setRowCount(i+1);
              twl->setItem(i, 0, new QTableWidgetItem(lst[0]));//ip

          }
    });
/***********************************************************************/

twl->setFixedSize(QSize(boy*13+5,boy*6));
twl->setColumnCount(1);
//twl->setRowCount(0);
twl->setColumnWidth(0, 400);

twl->setHorizontalHeaderItem(0, new QTableWidgetItem("Web Adresi"));
twl->setSelectionBehavior(QAbstractItemView::SelectRows);
twl->setSelectionMode(QAbstractItemView::SingleSelection);
//connect(tw, &QTableWidget::cellClicked, this, cellClicked());
connect(twl, SIGNAL(cellDoubleClicked(int,int)),SLOT(cellDoubleClicked(int,int)));
twl->setRowCount(0);
QStringList list=fileToList("/usr/share/e-ag/","webblocklist");
for(int i=0;i<list.count();i++)
{
    QString line=list[i];
    QStringList lst=line.split("|");
    twl->setRowCount(twl->rowCount()+1);
    twl->setItem(i, 0, new QTableWidgetItem(lst[0]));//ip

}

/***************************************************************************/

QFont f1( "Arial", 8, QFont::Normal);
webblockcb->setFont(f1);
webblockcb->setChecked(webblockstate);
/// qDebug()<<"copystate1"<<copyState;
connect(webblockcb, &QCheckBox::clicked, [=]() {
    webblockstate=webblockcb->checkState();
   // qDebug()<<copyState;
});



    QToolButton *ayarKaydetButton= new QToolButton;
    ayarKaydetButton->setFixedSize(QSize(boy*3,boy*2));
    ayarKaydetButton->setIconSize(QSize(boy,boy));
    ayarKaydetButton->setStyleSheet("Text-align:center");
    ayarKaydetButton->setIcon(QIcon(":/icons/save.svg"));
    ayarKaydetButton->setAutoRaise(true);
    ayarKaydetButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ayarKaydetButton->setFont(f2);
    ayarKaydetButton->setText("Ayarları Kaydet");

    connect(ayarKaydetButton, &QPushButton::clicked, [=]() {
        /*kontrol();
        if (status==false) return;
/**************************************************************/
          bool drmm1=false;

          /***********************e-ag-x11client.conf oluşturuluyor***********************************/
          QStringList ayarlist;
          ayarlist.append("kilitstate|"+QString::number(kilitstate));
          ayarlist.append("webblockstate|"+QString::number(webblockstate));
          listToFile("/usr/share/e-ag/",ayarlist,"e-ag-x11client.conf");
          webBlockAktifPasif();
         /* QString mesaj="webblock|"+QString::number(webblockstate)+"||||||";
          udpGuiServerSend = new QUdpSocket();
          QByteArray datagram = mesaj.toUtf8();// +QHostAddress::LocalHost;
          udpGuiServerSend->writeDatagram(datagram,QHostAddress::LocalHost, 51512);
           /***************************************************************************/
          QString sonuc=myMessageBox("e-ag", "\n"
                                             "\nAyarlar Kaydedildi..\n"
                                             "\n","","","tamam",QMessageBox::Information);

    });



    QCheckBox *kilitcb= new QCheckBox("Her Açılışta Ekranı Kilitle.",ayarPage);
    //QFont f1( "Arial", 8, QFont::Normal);
    kilitcb->setFont(f1);
    kilitcb->setChecked(kilitstate);
   /// qDebug()<<"copystate1"<<copyState;
    connect(kilitcb, &QCheckBox::clicked, [=]() {
        kilitstate=kilitcb->checkState();
       // qDebug()<<copyState;
});


    auto layout = new QGridLayout(ayarPage);
    layout->setContentsMargins(0, 0, 0,0);
    layout->addWidget(webadres, 2,0,1,1);
    layout->addWidget(webadresEkleButton, 2,1,1,1);

    layout->addWidget(twl, 5,0,1,1);
    layout->addWidget(webblockcb, 6,0,1,1);
   // layout->addWidget(webAyarKaydetButton, 7,0,1,1);


   // layout->addWidget(gelenKomut, 10,0,1,1);

    layout->addWidget(kilitcb,20,0,1,3);
    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(ayarKaydetButton);
    layout->addLayout(layout1, 30, 0,1,3,Qt::AlignHCenter);


///layout->setColumnStretch(6, 255);

    return ayarPage;
}


void MainWindow::cellDoubleClicked(int iRow, int iColumn)
{
     QString webadres= twl->item(iRow, 0)->text();
     QStringList list=fileToList("/usr/share/e-ag/","webblocklist");
/******************************************************************/
    //QMessageBox::StandardButton reply;
    // reply = QMessageBox::question(this, "Uyarı", "Bilgisayar Silinecek! Emin misiniz?",
      //                             QMessageBox::Yes|tr(QMessageBox::No);
     QMessageBox messageBox(this);
     messageBox.setText("Uyarı");
     messageBox.setInformativeText("Web Adresi İçin İşlem Seçiniz!");
     QAbstractButton *evetButton =messageBox.addButton(tr("Sil"), QMessageBox::ActionRole);
     QAbstractButton *hayirButton =messageBox.addButton(tr("Düzenle"), QMessageBox::ActionRole);
     messageBox.setIcon(QMessageBox::Question);
             messageBox.exec();
             if (messageBox.clickedButton() == evetButton) {
                 // qDebug()<<"evet basıldı";
                 list=listRemove(list,webadres);
                // komutSudoExpect("sudo iptables -I OUTPUT -m tcp -p tcp -d "+webadres+" --dport 443 -j ACCEPT",rootusername,rootpassword);
                 listToFile("/usr/share/e-ag/",list,"webblocklist");
                 if(list.count()==0) twl->setRowCount(0);
                 for(int i=0;i<list.count();i++)
                 {
                     QString line=list[i];
                     QStringList lst=line.split("|");
                     twl->setRowCount(i+1);
                     twl->setItem(i, 0, new QTableWidgetItem(lst[0]));//ip
                 }

                  webBlockAktifPasif();//// burası çok önemli
             }
             if (messageBox.clickedButton() == hayirButton) {
                 //qDebug()<<"hayır basıldı";
             }


}

#endif // AYAR_H
