#ifndef NETWORKWIDGET_H
#define NETWORKWIDGET_H

void MainWindow::networkProfil()
{
    // qDebug()<<"ayar click";
    QDialog * d = new QDialog();
    d->setWindowTitle("Ağ Profil Listesi");
    d->setFixedSize(QSize(boy*200,boy*50));
    d->setStyleSheet("font-size:"+QString::number(font.toInt()-2)+"px;");
    auto appIcon = QIcon(":/icons/e-ag.svg");
    d->setWindowIcon(appIcon);
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width() - d->width())/2;
    int y = (screenGeometry.height() - d->height()) / 2;
    d->move(x, y);
    /***********************************************************************/
    QTableWidget *twlh=new QTableWidget;

    twlh->setFixedSize(QSize(boy*195,boy*35));
    twlh->setColumnCount(15);
    //twlh->setRowCount(0);
    twlh->setHorizontalHeaderItem(0, new QTableWidgetItem("Seçili Ağ"));
    twlh->setHorizontalHeaderItem(1, new QTableWidgetItem("Ağ No"));
    twlh->setHorizontalHeaderItem(2, new QTableWidgetItem("AğAdı"));
    twlh->setHorizontalHeaderItem(3, new QTableWidgetItem("Server Ip"));
    twlh->setHorizontalHeaderItem(4, new QTableWidgetItem("Yayın Adresi"));
    twlh->setHorizontalHeaderItem(5, new QTableWidgetItem("Ip"));
    twlh->setHorizontalHeaderItem(6, new QTableWidgetItem("Mac"));
    twlh->setHorizontalHeaderItem(7, new QTableWidgetItem("Port"));
    twlh->setHorizontalHeaderItem(8, new QTableWidgetItem("ftpPort"));
    twlh->setHorizontalHeaderItem(9, new QTableWidgetItem("rootpath"));
    twlh->setHorizontalHeaderItem(10, new QTableWidgetItem("language"));
    twlh->setHorizontalHeaderItem(11, new QTableWidgetItem("Ekran Kilidi"));
    twlh->setHorizontalHeaderItem(12, new QTableWidgetItem("Web Filtresi"));
    twlh->setHorizontalHeaderItem(13, new QTableWidgetItem(""));
    twlh->setHorizontalHeaderItem(14, new QTableWidgetItem(""));


    twlh->setSelectionBehavior(QAbstractItemView::SelectRows);
    twlh->setSelectionMode(QAbstractItemView::SingleSelection);
    //connect(tw, &QTableWidget::cellClicked, this, cellClicked());
    twlh->setRowCount(0);
    twlh->setColumnWidth(0, boy*10);
    twlh->setColumnWidth(1, boy*10);
    twlh->setColumnWidth(2,boy*10);
    twlh->setColumnWidth(3,boy*15);
    twlh->setColumnWidth(4,boy*15);
    twlh->setColumnWidth(5,boy*10);
    twlh->setColumnWidth(6,boy*10);
    twlh->setColumnWidth(7,boy*10);
    twlh->setColumnWidth(8,boy*10);
    twlh->setColumnWidth(9,boy*10);
    twlh->setColumnWidth(10,boy*10);
    twlh->setColumnWidth(11,boy*20);
    twlh->setColumnWidth(12,boy*20);
    twlh->setColumnWidth(13,boy*10);
    twlh->setColumnWidth(14,boy*10);


    DatabaseHelper *db=new DatabaseHelper(localDir+"e-ag.json");
    QJsonArray dizi=db->Oku();
    int sr=0;

    for (const QJsonValue &item : dizi) {
        QJsonObject veri=item.toObject();

        twlh->setRowCount(twlh->rowCount()+1);
        QCheckBox *mCheck = new QCheckBox();
        mCheck->setFixedWidth(boy*15);
        mCheck->setChecked(false);
        QLineEdit * networkIndex = new QLineEdit();
        QLineEdit * networkName = new QLineEdit();
        QLineEdit * serverAddress = new QLineEdit();
        QLineEdit * networkBroadCastAddress = new QLineEdit();
        QLineEdit * ipAddress = new QLineEdit();
        QLineEdit * macAddress = new QLineEdit();
        QLineEdit * networkTcpPort = new QLineEdit();
        QLineEdit * ftpPort = new QLineEdit();
        QLineEdit * rootPath= new QLineEdit();
        QLineEdit * language = new QLineEdit();
        QCheckBox * lockScreenState = new QCheckBox();
        QCheckBox * webblockState = new QCheckBox();


        QToolButton *savetButton= new QToolButton;
        savetButton->setText("Kaydet");
        savetButton->setFixedWidth(boy*10);
        connect(savetButton, &QPushButton::clicked, [=]() {
            //qDebug()<<"Değişiklikler Kaydedildi.."<<networkInsertButton->toolTip();
            int numRows = twlh->rowCount();
            for ( int i = 0 ; i < numRows ; i++)
            {

                if (networkIndex->text()==savetButton->toolTip())
                {
                    QJsonArray dizi=db->Ara("networkIndex",savetButton->toolTip());
                    if(dizi.count()>0)
                    {
                        QCheckBox* mBox1 = static_cast<QCheckBox*> (twlh->cellWidget(i,0));
                        QLineEdit * networkIndex1 = static_cast<QLineEdit*> (twlh->cellWidget(i,1));
                        QLineEdit * networkName1 = static_cast<QLineEdit*> (twlh->cellWidget(i,2));
                        QLineEdit * serverAddress1 =static_cast<QLineEdit*> (twlh->cellWidget(i,3));
                        QLineEdit * networkBroadCastAddress1 =static_cast<QLineEdit*> (twlh->cellWidget(i,4));
                        QLineEdit * ipAddress1 =static_cast<QLineEdit*> (twlh->cellWidget(i,5));
                        QLineEdit * macAddress1 =static_cast<QLineEdit*> (twlh->cellWidget(i,6));

                        QLineEdit * networkTcpPort1 = static_cast<QLineEdit*> (twlh->cellWidget(i,7));
                        QLineEdit * ftpPort1 = static_cast<QLineEdit*> (twlh->cellWidget(i,8));
                        QLineEdit * rootPath1 = static_cast<QLineEdit*> (twlh->cellWidget(i,9));
                        QLineEdit * language1 = static_cast<QLineEdit*> (twlh->cellWidget(i,10));
                        QCheckBox * lockScreenState1 = static_cast<QCheckBox*> (twlh->cellWidget(i,11));
                        QCheckBox * webblockState1 = static_cast<QCheckBox*> (twlh->cellWidget(i,12));

                        qDebug()<<"Ağ Profili Değiştirilecek."<<savetButton->toolTip();
                        QJsonObject veri;
                        if (mBox1->isChecked()) veri["selectedNetworkProfil"] =true;
                        else veri["selectedNetworkProfil"] =false;
                        veri["networkIndex"] = networkIndex1->text();
                        veri["networkName"] = networkName1->text();
                        veri["networkTcpPort"] = networkTcpPort1->text();
                        veri["networkBroadCastAddress"]=networkBroadCastAddress1->text();//dikkat
                        veri["serverAddress"]=serverAddress1->text();
                        veri["ipAddress"]=ipAddress1->text();
                        veri["macAddress"]=macAddress1->text();
                        veri["ftpPort"]=ftpPort1->text();
                        veri["rootPath"]=rootPath1->text();
                        veri["language"]=language1->text();
                        //veri["lockScreenState"]=lockScreenState->text();
                        //veri["webblockState"]=webblockState->text();
                        if (lockScreenState1->isChecked()) veri["lockScreenState"] =true;
                        else veri["lockScreenState"] =false;
                        if (webblockState1->isChecked()) veri["webblockState"] =true;
                        else veri["webblockState"] =false;
                        //qDebug()<<"network kayıt"<<veri;
                        db->Sil("networkIndex",networkIndex1->text());
                        db->Ekle(veri);
                    }
                }
            }
            d->close();
            networkProfil();
            networkProfilLoad();
        });
        QToolButton *networkRemoveButton= new QToolButton;
        networkRemoveButton->setText("Sil");
        networkRemoveButton->setFixedWidth(boy*10);
        connect(networkRemoveButton, &QPushButton::clicked, [=]() {
            //qDebug()<<"Profil Silindi.."<<networkRemoveButton->toolTip();
            QJsonArray dizi=db->Ara("networkIndex",networkRemoveButton->toolTip());
            qDebug()<<"Ağ Profili Silinecek."<<networkRemoveButton->toolTip();
            db->Sil("networkIndex",networkIndex->text());
            d->close();
            networkProfil();
            networkProfilLoad();
        });

        networkIndex->setText(veri.value("networkIndex").toString());
        networkIndex->setReadOnly(true);
        networkName->setText(veri.value("networkName").toString());
        networkBroadCastAddress->setText(veri.value("networkBroadCastAddress").toString());
        serverAddress->setText(veri.value("serverAddress").toString());
        ipAddress->setText(veri.value("ipAddress").toString());
        macAddress->setText(veri.value("macAddress").toString());

        networkTcpPort->setText(veri.value("networkTcpPort").toString());
        ftpPort->setText(veri.value("ftpPort").toString());
        rootPath->setText(veri.value("rootPath").toString());
        language->setText(veri.value("language").toString());

        savetButton->setToolTip(networkIndex->text());
        twlh->setCellWidget(sr,0,mCheck);
        twlh->setCellWidget(sr,1,networkIndex);
        twlh->setCellWidget(sr,2,networkName);
        twlh->setCellWidget(sr,3,serverAddress);
        twlh->setCellWidget(sr,4,networkBroadCastAddress);
        twlh->setCellWidget(sr,5,ipAddress);
        twlh->setCellWidget(sr,6,macAddress);
        twlh->setCellWidget(sr,7,networkTcpPort);
        twlh->setCellWidget(sr,8,ftpPort);
        twlh->setCellWidget(sr,9,rootPath);
        twlh->setCellWidget(sr,10,language);
        twlh->setCellWidget(sr,11,lockScreenState);
        twlh->setCellWidget(sr,12,webblockState);
        twlh->setCellWidget(sr,13,savetButton);
        twlh->setCellWidget(sr,14,networkRemoveButton);
        if(veri.value("selectedNetworkProfil").toBool()) mCheck->setChecked(true);
        else mCheck->setChecked(false);
        if(veri.value("lockScreenState").toBool()) lockScreenState->setChecked(true);
        else lockScreenState->setChecked(false);
        if(veri.value("webblockState").toBool()) webblockState->setChecked(true);
        else webblockState->setChecked(false);
        //lockScreenState->setText(veri.value("lockScreenState").toString());
        //webblockState->setText(veri.value("webblockState").toString());

        sr++;
    }

    /********************************************************************/
    QToolButton *newNetworkButton= new QToolButton;
    newNetworkButton->setFixedSize(QSize(boy*40,boy*10));
    newNetworkButton->setIconSize(QSize(boy*40,boy*5));
    newNetworkButton->setStyleSheet("Text-align:center");
    newNetworkButton->setIcon(QIcon(":/icons/e-ag.svg"));
    newNetworkButton->setAutoRaise(true);
    newNetworkButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    // newNetworkButton->setFont(f2);
    newNetworkButton->setText("Yeni Profil Ekle");

    connect(newNetworkButton, &QPushButton::clicked, [=]() {
        DatabaseHelper *db=new DatabaseHelper(localDir+"e-ag.json");
        //qDebug()<<"broadcast address:"<<i<<ipmaclist[i].broadcast;
        QJsonObject veri;
        veri["networkIndex"] =QString::number(db->getIndex("networkIndex"));
        if(db->Oku().size()==0) veri["selectedNetworkProfil"] =true;
        else veri["selectedNetworkProfil"] =false;
        veri["networkName"] = "network";
        veri["networkTcpPort"] = "7879";
        hostAddressMacButtonSlot();
        veri["serverAddress"]="";
        veri["ipAddress"]=interfaceList[0].ip;
        veri["macAddress"]=interfaceList[0].mac;
        veri["networkBroadCastAddress"]=interfaceList[0].broadcast;
        veri["ftpPort"]="12345";
        veri["rootPath"]="/tmp/";
        veri["language"]="tr_TR";
        veri["lockScreenState"]=false;
        veri["webblockState"]=false;
        db->Ekle(veri);
        d->close();
        networkProfil();
        networkProfilLoad();
    });

    /*********************************************************************/
    /*********************************************************************/

    QVBoxLayout * vbox = new QVBoxLayout();
    vbox->addWidget(twlh);
    QHBoxLayout * hbox = new QHBoxLayout();
    hbox->addWidget(newNetworkButton);

    vbox->addLayout(hbox);

    d->setLayout(vbox);
    d->exec();


}

void MainWindow::networkProfilLoad()
{


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
        networkProfilLoad();
    }

    qDebug()<<"eagconf bilgileri farklı güncelleniyor.";
    system("systemctl restart e-ag-client-console.service");
    system("systemctl restart e-ag-client-networkprofil.service");
}

#endif // NETWORKWIDGET_H
